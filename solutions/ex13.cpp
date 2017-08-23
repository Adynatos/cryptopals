#include "helpers.hpp"
#include <iostream>

using namespace std;
int main()
{
    cout << urlform_encode("foo=bar&baz=qux&zap=zazzle") << endl;
    cout << urlform_decode(urlform_encode("foo=bar&baz=qux&zap=zazzle")) << endl;
}
