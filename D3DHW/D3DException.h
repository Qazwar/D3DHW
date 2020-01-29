//An implementation of exception
#pragma once

#include <exception>
#include <string>

class D3DException : public std::exception
{
public:
	D3DException(int line, const char* file) noexcept;
	const char* what() const noexcept override;	//pop out line number and filename
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;	//format exception info
private:
	int line;	//line number that exception emerge
	std::string file;	//filename that exception emerge
protected:
	mutable std::string whatBuffer;	//can be overwritten by inher class
};

