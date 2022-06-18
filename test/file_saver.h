#ifndef DC_FILE_SAVER_H
#define DC_FILE_SAVER_H

#include <stdio.h>

class FileSaver
{
public:
	FileSaver(int size, const char* filename, const char* suffix = "");

	void write(const char* buf, int len);

	void save();

private:
	FILE* _file{NULL};
	char* _buf;
	int _buf_size;
	int _buf_pos;
	bool _is_save{ false };
};

#endif