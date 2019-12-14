#include "pch.h"
#include "Log.h"
#include "Application.h"



char selectedType = 'a';

void Log::Debug(const std::string& msg, logType logType, const char* file, int line)
{
	static int count = 0;
	if (count == 0) {
		std::cout << "What Logs will be visible? (n = none, a = all, t = trace, w = warning, e = error )" << std::endl;
		count++;
		//std::cin >> selectedType;
	}
	if (selectedType == 'a') {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (selectedType == 'w' && logType == logType::WARNING) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (selectedType == 't' && logType == logType::TRACE) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (selectedType == 'e' && logType == logType::ERROR) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (selectedType == 'n') {

	}
}

bool Log::CheckGLError(const char* file, int line)
{
	GLenum err = glGetError();
	bool errorFound = false;
	while (err != GL_NO_ERROR)
	{

		switch (err) {
		case(GL_NO_ERROR):
			break;

		
		}



		std::cout << "GL ERROR(" << file << " ," << line << "): " <<
			glewGetErrorString(err) << std::endl;
		errorFound = true;
	}


	return errorFound;
}
