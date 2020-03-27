#include "_7iHttp.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string requestContent; 
    ifstream ifRequest("HttpResponse.txt");
    string requestLine;
    while (getline(ifRequest, requestLine))
        requestContent += requestLine + "\r\n";
    if (requestContent.length()) {
        requestContent.pop_back();
        requestContent.pop_back();
    }
    ifRequest.close();

    _7iHttp http(Http::Request);
    http.read(Http::Response, requestContent);
    http.setMessage("7iZ", "Handsome");

    cout << http.getHttpPackage(); // << endl;
    cout << "...";
    return 0; 
}
