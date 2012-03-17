#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

using namespace ktn;

int main(int argc, const char** argv) {
	setlocale(LC_CTYPE, "");

	String a(_T("水樹")), b(_T("奈々"));
	_tprintf(_T("%s: %d\n"), a.string(), a.length());

	_tprintf(_T("%c\n"), a[-1]);

	String c = a + b;
	c.out();
	(-c).out();

	_tprintf(_T("%d\n"), c.indexOf(_T('々')));
	_tprintf(_T("%d\n"), c.lastIndexOf(_T('水'), 2));
	_tprintf(_T("%s\n"), c.slice(2).string());
	_tprintf(_T("%s\n"), c.slice(0, -3).string());
	_tprintf(_T("%s\n"), c.substr(2, 2).string());

	String space(_T("  Nana  Mizuki   "));
	_tprintf(_T("'%s'\n"), space.toUpperCase().string());
	_tprintf(_T("'%s'\n"), space.toLowerCase().string());
	_tprintf(_T("'%s'\n"), space.trimLeft().string());
	_tprintf(_T("'%s'\n"), space.trimRight().string());
	_tprintf(_T("'%s'\n"), space.trim().string());
	_tprintf(_T("'%s'\n"), (space - L' ').string());

	String d = b * 7;
	d.out();

	d = b * -3;
	d.out();

#ifdef _MSC_VER
	system("pause");
#endif
	return 0;
}
