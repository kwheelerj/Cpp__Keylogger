#ifndef HELPER_H
#define HELPER_H

#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

namespace Helper
{
	template <class T>
	std::string ToString(const T&);

	struct DateTime
	{
		DateTime()
		{
			time_t sec;
			time(&sec);
			struct tm *info = localtime(&sec);

			d = info->tm_mday;
			m = info->tm_mon + 1;
			y = 1900 + info->tm_year;
			M = info->tm_min;
			H = info->tm_hour;
			S = info->tm_sec;
		}

		DateTime(int d, int m, int y, int H, int M, int S)
			: d(d), m(m), y(y), H(H), M(M), S(S) {}
		
		DateTime(int d, int m, int y) : d(d), m(m), y(y), H(0), M(0), S(0) {}

		DateTime Now() const
		{
			return DateTime();
		}

		int d, m, y, H, M, S;

		std::string GetDateString() const
		{
			/* dd.mm.yyyy */
			return	std::string( d < 10 ? "0" : "" ) + ToString(d) +
					std::string( m < 10 ? ".0" : "." ) + ToString(m) + "." +
					ToString(y);
		}

		std::string GetTimeString(const std::string &sep = ":") const
		{
			/* HH:MM:SS */
			return	std::string( H < 10 ? "0" : "" ) + ToString(H) + sep +
					std::string( M < 10 ? "0" : "" ) + ToString(M) + sep +
					std::string( S < 10 ? "0" : "" ) + ToString(S);
		}

		std::string GetDateTimeString(const std::string &sep = ":") const
		{
			return GetDateString() + "-" + GetTimeString(sep);
		}

	};

	template<class T>
	std::string ToString(const T &e)
	{
		std::ostringstream s;
		s << e;
		return s.str();
	}

	/* For debugging and logging */
	/* NOTE: Keylogger is not dependent on this function. */
	void WriteAppLog(const std::string &s)
	{
		std::ofstream file("AppLog.txt", std::ios::app);
		file << "[" << Helper::DateTime().GetDateTimeString() << "]"
			<< "\n" << s << std::endl;
		file.close();
	}
}

#endif /* HELPER_H */