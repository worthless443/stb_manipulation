What is `stb_manipulation`?
--------

A take on understanding how parsing image files works and organizing them into data structures. This is not a standalone project and is to be used for another project doing something with images. Bascially image manipulation but in a lower-level (Handling directly with raw bytes).

The main premise is to break down `stbi_load` and its underlying infrastructure that processes the images and returns `stbi_uc*`.

I've thus far wrote things so that it can be used speperately, however the core algorithm is copied and it goes close to working, but is not robust and does only suppoert a specific format of files, aka jpeg but not all jpegs. 
Handling JPEG files
------

The heavy work of parsing JEPG files and processing it lies in:

`load_jpeg_image()` which depends on `stbi__decode_jpeg_image()` which depends on  `stbi__process_scan_header()` and `stbi__parse_entropy_coded_data()`

### `stbi__decode_jpeg_image()`

`stbi__jepg*` is created by decoding bytes into possibly CMYK, YCCK, or RGB format using  `stbi__decode_jpeg_image()`, after that resampling should take place.
Resampling
: In the original implemenation, an uninitialized pointer to `stbi_uc` is placed, then it repositions like so, `output +i * j * img_y` and increments by `ctx->img_y` every allocation of the result of previously discussed. Then
depending on `n` and image channel size, we resample `out` differently either using `CMYK` or `YCCK` format. Howeer, as of this commit, I have only implemented the simplier RGB.

Resampler
: Resampling is done depending on `ystep` and `vs` per different instances of `stbi__resample`, and that depending on `img_n` and `n` which is `req_comp ? req_comp : z->s->img_n >= 3 ? 3 : 1;` depending on the parameter `req_comp`.

### Some Additional Notes

### Issues 

Conditions in `__resample()` is wrong, thus produces incomplete samples of `stbi_uc*`. Only its size is 2 where it should have been 3 [fixed]
