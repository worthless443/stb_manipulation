
// section based from worthless443  (end)
int *check_for_same(int *tree, int arr[])  {
	if(tree[0]==0) return tree;
	arr[0] =  tree[0];
	arr[1] = tree[1];
	int *arr_r ;
	int size, match;
	//if(arr[1] > 1 || arr[0] > 1) arr_r = check_for_same(tree+3, arr);
	arr_r = check_for_same(tree+3, arr);
	for(size=0;*(int*)(arr_r+size);size++); //printf("%d ", arr_r[size]);
	printf("%d\n",arr_r[size-1]);
	//printf("%d\n", size);
	//arr[0] = (arr_r[size]==tree[1]);
	//arr[1] = (arr_r[size-1]==tree[0]);

	//match = arr[0]==1 && arr[1]==0;
	//printf("%d\n", arr_r[0]);
	return tree;
} 

int *arr_without_zeros(int *tree, int size) {
	int *out = malloc(8*size);
	for(int i=0,j=0;i<size;i++) {
			if(tree[i]!=51) {
			out[j] = tree[i];
			j++;
		}
	}
	return out;
}

int *check_for_same_(int *tree, int **arr, int **out, int count)  {
	if(tree[0]==0) {
		return tree;
	}
	for(int i=1;*(tree+i);i++) { 
			if(tree[0] == tree[i])  tree[i] = 51;
	}
	arr[count] = tree;
	*out = arr[0]; // idk why save it as it is always at idx 0
	check_for_same_(tree+1, arr, out, ++count);
      	return tree;
}
int get_size(int *ptr) {
	int i=1;
	for(;*(ptr+i);i++);
	return i;
}

void print_tree(int *tree,int size) {
    	for(int k=0;k<size;k++) printf("%d ", tree[k]);
	printf("\n");
}

int check_same(int *tree) {
	int i;
	int *t;
	int size = get_size(tree);
	int *arr[size], *out;
	t = check_for_same_(tree,arr, &out,0);
	out = arr_without_zeros(out,size);
	
	//for(int i=0;i<get_size(tree);i++) if(out[i]==0) return 1;
	printf("%d : %d",get_size(out), get_size(tree));
	return (get_size(out)<get_size(t)) ? 1 : 0;
}

#define getSize(a) sizeof(a)/sizeof(int)
int check_zeros(int *arr) {
	int i=0,j=0;
	while(arr[i]!=-1) {
		if(arr[i]==0) {
			arr = arr + i;
			j++;
			i=0;
		}
		i++;
	}
	return j;
}
// section based from worthless443 (end)
//


