#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

const char *ssid = "ESP32_AP";

AsyncWebServer server(80);
DNSServer dnsServer;
class ModelData
{
private:
    char hostname[64];
    char path[64];
    char port[16];
    char chuki[64];
    char doSang[64];
    char top[16];
    char left[16];
    char right[16];
    char bott[16];
    char tenKH[64];
    char SDB[64];
    char idDevice[64];
    char PDN[64];

public:
    // Setters
    void setTenKH(const String &value)
    {
        snprintf(tenKH, sizeof(tenKH), "%s", value.c_str());
    }

    void setSDB(const String &value)
    {
        snprintf(SDB, sizeof(SDB), "%s", value.c_str());
    }

    void setIDDevice(const String &value)
    {
        snprintf(idDevice, sizeof(idDevice), "%s", value.c_str());
    }

    void setPDN(const String &value)
    {
        snprintf(PDN, sizeof(PDN), "%s", value.c_str());
    }

    void setHostname(const String &value)
    {
        snprintf(hostname, sizeof(hostname), "%s", value.c_str());
    }

    void setPath(const String &value)
    {
        snprintf(path, sizeof(path), "%s", value.c_str());
    }

    void setPort(const String &value)
    {
        snprintf(port, sizeof(port), "%s", value.c_str());
    }

    void setChuki(const String &value)
    {
        snprintf(chuki, sizeof(chuki), "%s", value.c_str());
    }

    void setDoSang(const String &value)
    {
        snprintf(doSang, sizeof(doSang), "%s", value.c_str());
    }

    void setTop(const String &value)
    {
        snprintf(top, sizeof(top), "%s", value.c_str());
    }

    void setLeft(const String &value)
    {
        snprintf(left, sizeof(left), "%s", value.c_str());
    }

    void setRight(const String &value)
    {
        snprintf(right, sizeof(right), "%s", value.c_str());
    }

    void setBott(const String &value)
    {
        snprintf(bott, sizeof(bott), "%s", value.c_str());
    }

    // Getters
    const char *getTenKH() const { return tenKH; }
    const char *getSDB() const { return SDB; }
    const char *getIDDevice() const { return idDevice; }
    const char *getPDN() const { return PDN; }
    const char *getHostname() const { return hostname; }
    const char *getPath() const { return path; }
    const char *getPort() const { return port; }
    const char *getChuki() const { return chuki; }
    const char *getDoSang() const { return doSang; }
    const char *getTop() const { return top; }
    const char *getLeft() const { return left; }
    const char *getRight() const { return right; }
    const char *getBott() const { return bott; }
};

ModelData model;

void saveToFile(const String &jsonString)
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    File file = SPIFFS.open("/config.json", "w");
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }

    file.print(jsonString);
    file.close();
    Serial.println("Data saved successfully!");
}

// String loadFromFile()
// {
//     if (!SPIFFS.begin(true))
//     {
//         Serial.println("SPIFFS Mount Failed");
//         return "{}";
//     }

//     File file = SPIFFS.open("/config.json", "r");
//     if (!file)
//     {
//         Serial.println("Failed to open file for reading");
//         return "{}";
//     }

//     String data;
//     while (file.available())
//     {
//         data += (char)file.read();
//     }
//     file.close();
//     return data;
// }

// Function to load data from SPIFFS and return a ModelData object
bool loadFromFile(ModelData &model) // note
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS Mount Failed");
        return false;
    }

    File file = SPIFFS.open("/config.json", "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.println("Failed to parse JSON");
        return false;
    }

    if (doc.containsKey("hostname"))
    {
        model.setHostname(doc["hostname"]);
    }
    if (doc.containsKey("path"))
        model.setPath(doc["path"].as<String>());
    if (doc.containsKey("port"))
        model.setPort(doc["port"]);
    if (doc.containsKey("chuki"))
        model.setChuki(doc["chuki"]);
    if (doc.containsKey("dosang"))
        model.setDoSang(doc["dosang"]);
    if (doc.containsKey("top"))
        model.setTop(doc["top"]);
    if (doc.containsKey("left"))
        model.setLeft(doc["left"]);
    if (doc.containsKey("right"))
        model.setRight(doc["right"]);
    if (doc.containsKey("bottom"))
        model.setBott(doc["bottom"]);
    if (doc.containsKey("tenKH"))
        model.setTenKH(doc["tenKH"]);
    if (doc.containsKey("SDB"))
        model.setSDB(doc["SDB"]);
    if (doc.containsKey("idDevice"))
        model.setIDDevice(doc["idDevice"]);
    if (doc.containsKey("PDN"))
        model.setPDN(doc["PDN"]);

    return true;
}

const char html_page[] PROGMEM = R"rawliteral(

)rawliteral";

void CheckValueExist(JsonDocument &doc, const String &input required, const char *keyName)
{
    if (!input required.isEmpty())
        doc[keyName] = input required.c_str();
}

void setup()
{
    Serial.begin(115200);
    WiFi.softAP(ssid);
    Serial.println("Access Point Started");
    dnsServer.start(53, "*", WiFi.softAPIP());

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->redirect("/"); });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  String page = FPSTR(html_page);
                  if(loadFromFile(model)) Serial.println(model.getHostname());
                  page.replace("%HOSTNAME%",loadFromFile(model) ? model.getHostname() : "http://14.224.158.56");
                  page.replace("%PATH%", loadFromFile(model) ? model.getPath(): "/donghonuoc/upload");
                  page.replace("%PORT%", loadFromFile(model) ? model.getPort(): "8081");
                  page.replace("%CHUKI%", loadFromFile(model) ? model.getChuki(): "1");
                  page.replace("%DOSANG%", loadFromFile(model) ? model.getDoSang(): "6");
                  page.replace("%TOP%", loadFromFile(model) ? model.getTop(): "6");
                  page.replace("%LEFT%", loadFromFile(model) ? model.getLeft(): "6");
                  page.replace("%RIGHT%", loadFromFile(model) ? model.getRight(): "6");
                  page.replace("%BOTTOM%", loadFromFile(model) ? model.getBott(): "9");
                  page.replace("%TENKH%", loadFromFile(model) ? model.getTenKH(): "VDTAS");
                  page.replace("%SDB%", loadFromFile(model) ? model.getSDB(): "19216811");
                  page.replace("%IDDEVICE%", loadFromFile(model) ? model.getIDDevice(): "abh");
                  page.replace("%PDN%", loadFromFile(model) ? model.getPDN(): "wt is that");
                  request->send(200, "text/html; charset=utf-8", page); });

    server.on("/data", HTTP_POST, [](AsyncWebServerRequest *request)
              {
        StaticJsonDocument<512> doc;

        // Serial.printf(model.getHostname());

        if (request->hasParam("tenKH", true)) CheckValueExist(doc,request->getParam("tenKH", true)->value(),"tenKH");
        if (request->hasParam("path", true)) CheckValueExist(doc,request->getParam("path", true)->value(),"path");
        if (request->hasParam("port", true)) CheckValueExist(doc,request->getParam("port", true)->value(),"port");
        if (request->hasParam("chuki", true)) CheckValueExist(doc,request->getParam("chuki", true)->value(),"chuki");
        if (request->hasParam("dosang", true)) CheckValueExist(doc,request->getParam("dosang", true)->value(),"dosang");
        if (request->hasParam("cropTop", true)) CheckValueExist(doc,request->getParam("cropTop", true)->value(),"top");
        if (request->hasParam("idDevice", true)) CheckValueExist(doc,request->getParam("idDevice", true)->value(),"idDevice");
        if (request->hasParam("cropLeft", true)) CheckValueExist(doc,request->getParam("cropLeft", true)->value(),"left");
        if (request->hasParam("hostname", true)) CheckValueExist(doc,request->getParam("hostname", true)->value(),"hostname");
        if (request->hasParam("cropRight", true)) CheckValueExist(doc,request->getParam("cropRight", true)->value(),"right");
        if (request->hasParam("cropBottom", true)) CheckValueExist(doc,request->getParam("cropBottom", true)->value(),"bottom");
        if (request->hasParam("SDB", true)) CheckValueExist(doc,request->getParam("SDB", true)->value(),"SDB");
        if (request->hasParam("PDN", true)) CheckValueExist(doc,request->getParam("PDN", true)->value(),"PDN");

        String jsonString;
        serializeJson(doc, jsonString);
        saveToFile(jsonString);

        Serial.println("Received and Saved JSON: " + jsonString);
        request->send(200, "text/html","i was here"); });

    server.begin();
}

void loop()
{
    dnsServer.processNextRequest();
}
