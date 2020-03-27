#include "_7iHttp.h"

std::vector<std::string> _7iHttp::getParams(std::string content, std::string separate) {
    std::vector<std::string> params;
    int lastPos = 0, foundPos;
    while ((foundPos = content.find(separate, lastPos)) != std::string::npos) {
        params.push_back(content.substr(lastPos, foundPos - lastPos));
        lastPos = foundPos + separate.length();
    }
    params.push_back(content.substr(lastPos, content.length() - lastPos));
    return params;
}

_7iHttp::_7iHttp(int type) {
    m_nType = type;
    m_strMethod  = "GET";
    m_strURI     = "/";
    m_strVersion = "HTTP/1.1";
    m_strStatus  = "200";
    m_strReason  = "OK";
}

bool _7iHttp::isPackage(std::string content) {
    std::vector<std::string> messages = getParams(content, "\r\n");
    if (messages.size() < 2)
        return false;

    std::vector<std::string> headers = getParams(*messages.begin(), " ");
    if (int(headers.size()) != 3)
        return false;

    for (int i = 1; i < int(messages.size()); ++i) {
        if (messages[i] == "")
            return true;
        if (messages[i].find(": ") == std::string::npos)
            return false;
    }

    return false;
}

void _7iHttp::setMethod(std::string method) {
    m_strMethod = method;
}

std::string _7iHttp::getMethod() {
    return m_strMethod;
}

void _7iHttp::setURI(std::string uri) {
    m_strURI = uri;
}

std::string _7iHttp::getURI() {
    return m_strURI;
}

void _7iHttp::setVersion(std::string version) {
    m_strVersion = version;
}

std::string _7iHttp::getVersion() {
    return m_strVersion;
}

void _7iHttp::setStatus(std::string status) { 
    m_strStatus = status;
}

std::string _7iHttp::getStatus() {
    return m_strStatus; 
}

void _7iHttp::setReason(std::string reason) {
    m_strReason = reason;
}

std::string _7iHttp::getReason() {
    return m_strReason;
}

std::string _7iHttp::getMessage(std::string name) {
    return m_arrMessages[name];
}

void _7iHttp::setMessage(std::string name, std::string value) {
    m_arrMessages[name] = value; 
}

void _7iHttp::readMessage(std::vector<std::string> messages) {
    for (int i = 0; i < int(messages.size()); ++i) {
        if (messages[i] == "") {
            std::string content;
            while ((++i) < int(messages.size()))
                content += messages[i] + "\r\n";
            m_strContent = content;
            break;
        }

        std::vector<std::string> message = getParams(messages[i], ": ");
        m_arrMessages[message[0]] = message[1];
    }
}

bool _7iHttp::read(int type, std::string requestContent) {
    if (!isPackage(requestContent))
        return false;

    m_nType = type;
    std::vector<std::string> messages = getParams(requestContent, "\r\n");
    std::vector<std::string> headers = getParams(*messages.begin(), " ");

    switch (m_nType)
    {
        case Http::Request:
            m_strVersion = headers[0];
            m_strStatus  = headers[1];
            m_strReason  = headers[2];
            break;

        case Http::Response:
            m_strMethod  = headers[0];
            m_strURI     = headers[1];
            m_strVersion = headers[2];
            break; 
    }

    m_arrMessages.clear();
    messages.erase(messages.begin());
    readMessage(messages);

    return true;
}

std::string _7iHttp::getHttpPackage() {
    std::string httpPackage;

    // Header
    if (m_nType == Http::Request) {
        httpPackage += m_strMethod + " ";
        httpPackage += m_strURI + " ";
        httpPackage += m_strVersion + "\r\n";
    } else {
        httpPackage += m_strVersion + " ";
        httpPackage += m_strStatus + " ";
        httpPackage += m_strReason + "\r\n";
    }

    // Messages
    for (std::pair<std::string, std::string> message : m_arrMessages)
        httpPackage += message.first + ": " + message.second + "\r\n";

    // Content
    httpPackage += "\r\n";
    httpPackage += m_strContent;
    return httpPackage; 
}
