#pragma once

const char* WcsToMbcs(const wchar_t* src);
const wchar_t* MbcsToWcs(const char* src);
const string& WstringToString(const wstring& src);
const wstring& StringToWstring(const string& src);

string& ltrim(string &s);
string& rtrim(string &s);
string& trim(string &s);

#define W2S WstringToString
#define S2W StringToWstring