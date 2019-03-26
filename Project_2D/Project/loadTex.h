#pragma once
#pragma once
#include <iostream>
#include <Gl/glut.h>
#include <windows.h>
#include <tchar.h>
#define MAX_CHAR       128
//#include ""
using namespace std;

#define BMP_Header_Length 54

int power_of_two(int nn)
{
	if (nn <= 0)
		return 0;
	return (nn & (nn - 1)) == 0;
}

GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// 打开文件，如果失败，返回  
	errno_t err;
	FILE* pFile;
	err = fopen_s(&pFile, file_name, "rb");
	if (err != 0) {
		cout << file_name << " not open" << endl;
		return 0;
	}


	// 读取文件中图象的宽度和高度  
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数  
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存  
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// 读取像素数据  
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放  
	// 若图像宽高超过了OpenGL规定的最大值，也缩放  
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形  
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// 计算每行需要的字节数和总字节数  
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// 分配内存  
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// 进行像素缩放  
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height  
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// 分配一个新的纹理编号  
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数  
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复  
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定  
	free(pixels);
	return texture_ID;
}

//存储像素信息
struct Pixel {
	GLubyte red, green, blue;
	int x, y;
	bool jun;
} pixel;
//读取像素 有色返回1,白色反回0
bool readPixel(int x, int y) {
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

	if ((int)pixel.red <255 || (int)pixel.blue <255 || (int)pixel.green <255) {
		//存在颜色
		pixel.jun = 1;
		//cout << (int)pixel.red << " " << (int)pixel.green << " " << (int)pixel.blue << endl;
	}
	else {
		pixel.jun = 0;
	}
	//cout << pixel.jun << endl;
	return pixel.jun;
}
//读取像素 指定颜色返回1,否则反回0
bool readPixelRGB(int x, int y, int red, int green, int blue) {
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

	if ((int)pixel.red == red && (int)pixel.blue == blue && (int)pixel.green == green) {
		//存在颜色
		pixel.jun = 1;
		//cout << (int)pixel.red << " " << (int)pixel.green << " " << (int)pixel.blue << endl;
	}
	else {
		pixel.jun = 0;
	}
	//cout << pixel.jun << endl;
	return pixel.jun;
}

//void read_png(char *file_name) {
//	png_structp png_ptr;
//	png_infop info_ptr;
//	png_uint_32 width, height;
//	int bit_depth, color_type, interlace_type;
//	FILE *fp;
//	if ((fp = fopen(file_name, "rb")) == NULL) {
//		return (ERROR);
//	}
//
//	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
//		0, 0, 0);
//	if (png_ptr == NULL) {
//		fclose(fp);
//		return (ERROR);
//	}
//	info_ptr = png_create_info_struct(png_ptr);
//	if (info_ptr == NULL) {
//		fclose(fp);
//		png_destroy_read_struct(&png_ptr, NULL, NULL);
//		return (ERROR);
//	}
//	if (setjmp(png_jmpbuf(png_ptr))) {
//		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
//		fclose(fp);
//		return (ERROR);
//	}
//	png_init_io(png_ptr, fp);
//	png_read_info(png_ptr, info_ptr);
//	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
//		&color_type, &interlace_type, NULL, NULL);
//	if (color_type == PNG_COLOR_TYPE_PALETTE) {
//		png_set_palette_to_rgb(png_ptr);
//	}
//	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
//		png_set_expand_gray_1_2_4_to_8(png_ptr);
//	}
//	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) != 0) {
//		png_set_tRNS_to_alpha(png_ptr);
//	}
//	png_read_update_info(png_ptr, info_ptr);
//	png_bytep row_pointers[height];
//	int row, col;
//	for (row = 0; row < height; row++) {
//		row_pointers[row] = NULL;
//	}
//	for (row = 0; row < height; row++) {
//		row_pointers[row] = png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
//	}
//	png_read_image(png_ptr, row_pointers);
//	GLubyte * rgba = (GLubyte *)malloc(width * height * 4);
//	int pos = (width * height * 4) - (4 * width);
//	for (row = 0; row < height; row++) {
//		for (col = 0; col < (4 * width); col += 4) {
//			rgba[pos++] = row_pointers[row][col];
//			rgba[pos++] = row_pointers[row][col + 1];
//			rgba[pos++] = row_pointers[row][col + 2];
//			rgba[pos++] = row_pointers[row][col + 3];
//		}
//		pos = (pos - (width * 4) * 2);
//	}
//	//glEnable(GL_TEXTURE_2D);
//	glGenTextures(1, &textureID);
//	printf("%d\n", textureID);
//	//glPixeStorei(GL_UNPACK_ALIGNMENT, 1);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0
//		, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	free(rgba);
//	fclose(fp);
//}