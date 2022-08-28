#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "logger.h"

// TODO add a struct that enables use of doing pretty much anything done here right now
stbi__resample *get_resampling(stbi__context *ctx, stbi__jpeg *j) {
	j->s = ctx;
	int decode_n;
	stbi__setup_jpeg(j);
	if(!stbi__decode_jpeg_image(j)) {
		printf("not a jpeg file\n");
		return NULL;
	}
	if(j->s->img_n) decode_n = j->s->img_n;
	else decode_n = 4;
	
	stbi__resample *rvec = stbi__malloc(sizeof(stbi__resample)*decode_n);
	for(int i=0;i<decode_n;i++) {
         stbi__resample *r = &rvec[i];
         // allocate line buffer big enough for upsampling off the edges
         // with upsample factor of 4
         j->img_comp[i].linebuf = (stbi_uc *) stbi__malloc(j->s->img_x + 3);
	 if(!j->img_comp[i].linebuf) printf("warning: overflow\n");

         r->hs      = j->img_h_max / j->img_comp[i].h;
         r->vs      = j->img_v_max / j->img_comp[i].v;
         r->ystep   = r->vs >> 1;
         r->w_lores = (j->s->img_x + r->hs-1) / r->hs;
         r->ypos    = 0;
         r->line0   = r->line1 = j->img_comp[i].data;
         if      (r->hs == 1 && r->vs == 1) r->resample = resample_row_1;
         else if (r->hs == 1 && r->vs == 2) r->resample = stbi__resample_row_v_2;
         else if (r->hs == 2 && r->vs == 1) r->resample = stbi__resample_row_h_2;
         else if (r->hs == 2 && r->vs == 2) r->resample = j->resample_row_hv_2_kernel;
         else                               r->resample = stbi__resample_row_generic;
 
	 int prev,curr;
	 if(i>0 && i < decode_n -1) {
	 	prev =  (*(rvec -i+1)).w_lores;
		curr = rvec[i].w_lores;
	 }
	 else prev = curr = rvec[i].w_lores;
      }

	return rvec;
}

void map_jpeg(stbi__resample *r, stbi__context *ctx, jpeg_mapper *mapper) {
	int i=0;
	int hsvs = ctx->img_y * ctx->img_x;
	while(r->hs!=0) {
		jpeg_mapper *m = &mapper[i];
		for(int j=r->hs;j<hsvs;j+=r->w_lores) {
				mapper->hs_mtl +=  j - r->w_lores ;
			}
		  	m->hs_mtl = (mapper->hs_mtl) * mapper->hs_mtl/(ctx->img_x*ctx->img_y);
			r++;
		i++;
		}
}

stbi_uc **__resample(stbi__jpeg *j, stbi__resample *resmp) {
	int i=1,k=0;
	int decode_n = j->s->img_n;
	stbi_uc *out[decode_n];
	stbi__resample *r = &resmp[0];
	while(1) {
		 r = &resmp[i];
		 int y_bot = r->ystep >= (r->vs>>1);
		 if(r->ystep>4) break; // bits per channel number
		 if(i<=4) { 
			 if(j->img_comp[i].linebuf!=NULL && r->line0 !=NULL && r->line1!=NULL) { 
				 printf("resample\n");
				 out[k] = r->resample(j->img_comp[i].linebuf,
					     y_bot ? r->line1 : r->line0,
					     y_bot ? r->line0 : r->line1,
					     r->w_lores, r->hs);
			 	k++;
			 }
			 else if(j->img_comp[i].linebuf !=NULL) { 
				 printf("linebuf\n");
				 stbi__resample *rr = r - 2;
				 if(rr->line1!=NULL && rr->line0 !=NULL)  {
				 out[k] = rr->resample(j->img_comp[i].linebuf,
					     y_bot ? rr->line1 : rr->line0,
					     y_bot ? rr->line0 : rr->line1,
					     rr->w_lores, rr->hs);
				  k++;
				 }
				 else printf("error: is null\n");
			 }
		 }
		 else if(r->ystep>r->vs) {
			 (++r)->ystep=0;	
		 }
		 else if(r->ystep==r->vs) { 
			 (++r)->ystep = (--r)->ystep + (++r)->ystep + 1;
		 }
		 if(i>20) break;
		 i++;
	}

	stbi_uc **output  = stbi__malloc(sizeof(out));
	memcpy(output,out, sizeof(out));
	return output;
}


void print_mapper(jpeg_mapper *mapper) {
	while(mapper->hs_mtl>0) {
		printf("%d\n", mapper->hs_mtl);
		mapper++;
	}
}

//NOTE r->resample() is doing most of the heavy lifting
stbi_uc *__output(stbi__jpeg *r, stbi_uc **u) {
	int n = r->s->img_y ;
	stbi_uc *output = stbi__malloc(sizeof(stbi_uc)*n*n); 
	for(int i=0;i<r->s->img_y;i++) {
		stbi_uc *out = output + i * r->s->img_y;
		for(int j=0;j<r->s->img_x-100;j++) {
			out[0] = u[0][j];
			out[1] = u[1][j];
			out[2] = u[2][j];
			out[3] = 255;
			out+=r->s->img_y; // such magic 
		}
		memcpy(output, out, n);
	}

	return output;
}
typedef struct {
	stbi__jpeg *j;
	int count_sof, count_eoi, count_soi;
} stbi__count_ref;

static void setup__j(stbi__jpeg *j, stbi__context *ctx) {
	j->s = ctx;
	stbi__setup_jpeg(j);
	int m;
	for(m=0;m<=3;m++) {
		j->img_comp[m].raw_data = NULL;
		j->img_comp[m].raw_coeff = NULL;
	}
   	j->restart_interval = 1;
   	j->jfif = 0;
   	j->app14_color_transform = -1; // valid values are 0,1,2
   	j->marker = STBI__MARKER_none; // initialize cached marker to empty
}

static int jpeg__getc(FILE *f,FILE *fp, stbi__count_ref *ref) {
	stbi__context ctx;	
	int m, count_sof=0, count_eoi=0,count_soi=0;
	int err_sof=0,err_eoi=0,err_soi=0;
	stbi__jpeg *j = stbi__malloc(sizeof(stbi__jpeg));

	stbi__start_file(&ctx, f);
	setup__j(j,&ctx);

   	m = stbi__get_marker(j);
	while(1) {
		if(!stbi__SOF(m))  {
			int ret = stbi__process_marker(j,m); //err check on this, probally errors out!
			if(!ret) err_sof++;
			m = stbi__get_marker(j);
			count_sof++;
		}
		else break;
	}

	fseek(f,0,SEEK_SET);
	stbi__start_file(&ctx, f);
	setup__j(j,&ctx);
	while(1) {
		if(!stbi__SOI(m))  {
			int ret = stbi__process_marker(j,m); //err check on this, probally errors out!
			if(!ret) err_soi++;
			m = stbi__get_marker(j);
			count_soi++;
		}
		else break;
	}

	fseek(f,0,SEEK_SET);
	stbi__start_file(&ctx, f);
	setup__j(j,&ctx);
	while(1) {
		if(!stbi__EOI(m))  {
			int ret = stbi__process_marker(j,m); //err check on this, probally errors out!
			if(!ret) err_eoi++;
			m = stbi__get_marker(j);
			count_eoi++;
		}
		else break;
	}
	ref->count_soi = count_soi;
	ref->count_sof = count_sof;
	ref->count_eoi = count_eoi;

	if(err_soi) my_log(fp, err_soi, 0);
	if(err_eoi) my_log(fp, err_eoi, 0);
	if(err_sof) my_log(fp, err_sof, 0);
	fseek(f,0,SEEK_SET);
	return 1;
}
// non static
const char *stbi_parse(const char *fname) {
	//int count = jpeg__get_count();
	//return NULL;
	stbi__context ctx ;
	stbi__result_info ri;
	stbi__count_ref cref;
	stbi__jpeg *j = stbi__malloc(sizeof(stbi__jpeg));
	jpeg_mapper *mapper = stbi__malloc(sizeof(jpeg_mapper)*10);
	int w=100,h=100,cmp=2;
	FILE *f = stbi__fopen(fname, "rb");
	FILE *fp = logger_start_f("test.log");
	//const char *out = stbi__jpeg_load(&ctx, &w,&h,&cmp,1, &ri);
	jpeg__getc(f,fp,&cref);
	my_log(fp,0,0);
	return NULL;
	stbi__start_file(&ctx,f);
	setup_result(&ri);

	stbi__resample *svec = get_resampling(&ctx,j);

	return NULL;

	stbi_uc **out = __resample(j, svec);

	if(out[0] == NULL && out[1] && out[2]==NULL) { 
		printf("is null\n");
		return NULL;
	}
	
	stbi_uc  *output = __output(j,out);

	for(int i=0, k=0;i<800 && output[i]!=0;i++,k++) {
		printf("%d ", output[i]);
		if(k==10) {
			printf("\n");
			k=0;
		}
	}
	//map_jpeg(svec, &ctx, mapper);
	////print_mapper(mapper);
	//printf("%d\n", mapper.hs_mtl);
	return NULL;
}

