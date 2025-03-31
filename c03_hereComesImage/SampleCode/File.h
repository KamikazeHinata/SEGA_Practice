#ifndef INCLUDED_FILE_H
#define INCLUDED_FILE_H

class File
{
public:
	File(const char* filename);
	~File();
	int Size() const;
	const char* Data() const;
	unsigned GetUnsigned(int p) const;
private:
	int mSize;
	char* mData;
};

#endif