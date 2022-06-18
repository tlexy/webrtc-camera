#include "file_saver.h"
#include <string.h>
#include <string>
#include <time.h>

FileSaver::FileSaver(int size, const char* filename, const char* suffix)
{
	_buf = new char[size];
	_buf_size = size;
	_buf_pos = 0;

	std::string name(filename);
	int now = time(NULL);
	name = name + std::to_string(now) +std::string(suffix);
	_file = fopen(name.c_str(), "wb");

}

void FileSaver::save()
{
	if (_is_save)
	{
		return;
	}
	_is_save = true;
	if (_file)
	{
		fwrite(_buf, 1, _buf_pos, _file);
		fclose(_file);
	}
}

void FileSaver::write(const char* buf, int len)
{
	if (_is_save)
	{
		return;
	}
	if (_buf_pos + len > _buf_size)
	{
		save();
	}
	else
	{
		memcpy(_buf + _buf_pos, buf, len);
		_buf_pos += len;
	}
}