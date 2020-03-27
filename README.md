# _7iHttp
Http Parser for C++11

### To create a http parser use:
``` c++
_7iHttp parser(mode = Http::Request /* or Http::Response */)
```

### To parser a HTTP Request:
``` c++
string requestContent;
parser.read(Http::Request /* or Http::Response */, requestContent);
```

### To pack it to string:
``` c++
string content = parser.getHttpPackage();
```
