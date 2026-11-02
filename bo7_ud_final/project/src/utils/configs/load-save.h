#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <pplwin.h>
#include <stdio.h>
#include "../protection/ex.h"

BOOL WritePrivateProfileInt ( LPCSTR lpAppName, LPCSTR lpKeyName, int nInteger, LPCSTR lpFileName ) {
	char lpString [ 1024 ];
	sprintf ( lpString, "%d", nInteger );
	return WritePrivateProfileStringA ( lpAppName, lpKeyName, lpString, lpFileName );
}
BOOL WritePrivateProfileFloat ( LPCSTR lpAppName, LPCSTR lpKeyName, float nInteger, LPCSTR lpFileName ) {
	char lpString [ 1024 ];
	sprintf ( lpString, "%f", nInteger );
	return WritePrivateProfileStringA ( lpAppName, lpKeyName, lpString, lpFileName );
}
float GetPrivateProfileFloat ( LPCSTR lpAppName, LPCSTR lpKeyName, FLOAT flDefault, LPCSTR lpFileName )
{
	char szData [ 32 ];

	GetPrivateProfileStringA ( lpAppName, lpKeyName, std::to_string ( flDefault ).c_str ( ), szData, 32, lpFileName );

	return ( float ) atof ( szData );
}

std::vector<std::string> GetList ( )
{
	std::vector<std::string> configs;
	WIN32_FIND_DATA ffd;
	LPCSTR directory = skCrypt ( "C:\\DefaultPath\\Call_Of_Duty\\*" );

	auto hFind = FindFirstFile ( directory, &ffd );
	if ( hFind == INVALID_HANDLE_VALUE ) {
		return configs;
	}

	do {
		if ( !( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
			std::string file_name = ffd.cFileName;
			if ( file_name.size ( ) < 4 ) 
				continue;

			std::string end = file_name.substr ( file_name.size ( ) - 4 ); 
			if ( end != skCrypt ( ".ini" ).decrypt ( ) )
				continue;

			file_name.erase ( file_name.end ( ) - 4, file_name.end ( ) );
			configs.push_back ( file_name );
		}
	} while ( FindNextFile ( hFind, &ffd ) != 0 );

	FindClose ( hFind ); 
	return configs;
}