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
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Đồng hồ nước</title>
    <style>
        .main-container {
            display: flex;
            flex-direction: column;
            gap: 8px;
            width: 350px;
            margin: 0 auto;
        }

        .Container {
            padding: 8px;
            display: flex;
            flex-wrap: wrap;
            gap: 8px;
        }

        .BoxInput {
            flex-grow: 1;
            width: 500px;
            height: 30px;
            display: flex;
            flex-basis: 600px;
            justify-content: space-between;
        }

        .title {
            color: rgb(14, 14, 207);
            text-align: center;
        }

        .ContainerBoxInput .inputItem {
            width: 50px;
        }

        .ContainerBoxInput {
            width: 170px;
        }


        .tab {
            display: flex;
            border-bottom: 2px solid #ccc;
            background-color: #f1f1f1;
        }

        .tab-button {
            width: 50%;
            background-color: inherit;
            border: none;
            outline: none;
            padding: 14px 20px;
            cursor: pointer;
            transition: background-color 0.3s;
            font-size: 16px;
            border-bottom: 2px solid transparent;
        }

        .tab-button:hover {
            background-color: #ddd;
        }

        .tab-button.active {
            border-bottom: 2px solid #007bff;
            font-weight: bold;
            background-color: #fff;
        }

        .tab-form {
            display: none;
        }

        .tab-form.form-active {
            display: block;
        }

        .btn-check {
            background-color: #90ee90;
            /* Light green */
            color: #1e4620;
            /* Dark green text for contrast */
            padding: 10px 20px;
            font-size: 16px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            transition: background-color 0.3s, box-shadow 0.2s, transform 0.1s;
            box-shadow: 0 2px 5px rgba(144, 238, 144, 0.5);
            width: 100%;
            display: flex;
            justify-content: center;
        }
    </style>
</head>

<body>
    <div class="main-container">
        <h2 class="title">Đồng hồ nước</h2>
        <div class="tab">
            <button class="tab-button active">Setting config</button>
            <button class="tab-button">Get status</button>
        </div>

        <form class="tab-form form-active" action="/data" method="post">
            <h3>Cấu hình server</h3>
            <div class="Container">
                <div class="BoxInput"><span>Hostname</span><input required value="%HOSTNAME%" name="hostname"
                        type="text" placeholder="vd:example.com"></div>
                <div class="BoxInput"><span>Path:</span><input required value="%PATH%" name="path" type="text"
                        placeholder="vd:/sub/nest"></div>
                <div class="BoxInput"><span>Port:</span><input required value="%PORT%" name="port" type="text"
                        placeholder="0">
                </div>
                <div class="BoxInput">
                    <span>Chu kì ảnh tải lên:</span>
                    <div class="ContainerBoxInput">
                        <input required style="width: 62%; height: 80%;" class="inputItem" name="chuki" type="text"
                            placeholder="0">
                        <select style="height: 96%;" name="donvichuki" id="">
                            <option value="86400">Ngày</option>
                        </select>
                    </div>
                </div>
                <div class="BoxInput"><span>Độ sáng khi chụp:</span><input required value="%DOSANG%" name="dosang"
                        type="text" placeholder="0"></div>
                <div class="BoxInput">
                    <label for="options">Loại đồng hồ:</label>
                    <select class="option-select" id="options" name="options">
                        <option value="">Select</option>
                        <option value="Zenner DN40">Zenner DN40</option>
                        <option value="George kent DN15">George kent DN15</option>
                    </select>
                </div>
                <div class="BoxInput"><span>Cắt ảnh(Top):</span><input required value="%TOP%" name="cropTop"
                        type="number" placeholder="0"></div>
                <div class="BoxInput"><span>Cắt ảnh(Left):</span><input required value="%LEFT%" name="cropLeft"
                        type="number" placeholder="0"></div>
                <div class="BoxInput"><span>Cắt ảnh(Right):</span><input required value="%RIGHT%" name="cropRight"
                        type="number" placeholder="0"></div>
                <div class="BoxInput"><span>Cắt ảnh(Bottom):</span><input required value="%BOTTOM%" name="cropBottom"
                        type="number" placeholder="0"></div>
            </div>
            <h3>Thông tin khách hàng</h3>
            <div class="Container">
                <div class="BoxInput"><span>Tên: </span><input required value="%TENKH%" name="tenKH" type="text"
                        placeholder="vd: toi la UwU"></div>
                <div class="BoxInput"><span>Số danh bộ: </span><input required value="%SDB%" name="SDB" type="text"
                        placeholder="vd: bột tôm"></div>
                <div class="BoxInput"><span>ID thiết bị: </span><input required value="%IDDEVICE%" name="idDevice"
                        type="text" placeholder="vd:UwU" disabled></div>
            </div>
            <h3>Cấu hình NB IOT</h3>
            <div class="Container">
                <div class="BoxInput"><span>Tên PDN: </span><input required value="%PDN%" name="PDN" type="text"
                        placeholder="vd: toi la UwU"></div>
            </div>
            <button type="submit">Submit</button>
        </form>

        <form class="tab-form">
            <div id="status-output">
                <p id="net-status">đăng kí mạng....</p>
                <p id="serial-status">kết nối serial....</p>
                <p id="image-status">kiểm tra gửi ảnh....</p>
                <p id="disconnect-status">bắt đầu ngắt kết nối trong .... giây.</p>
            </div>
            <div class="btn-check" onclick="startCheck()">Starting check</div>
        </form>
    </div>
</body>
<script>
    document.addEventListener("DOMContentLoaded", function () {

        const btn_check = document.querySelector('.btn-check');
        function startCheck() {
            fetch('/check') // replace with actual ESP IP
                .then(res => res.json())
                .then(data => {
                    // Update each status line
                    document.getElementById('net-status').textContent = data.network;
                    document.getElementById('serial-status').textContent = data.serial;
                    document.getElementById('image-status').textContent = data.image;
                    document.getElementById('disconnect-status').textContent = data.disconnect;
                })
                .catch(err => {
                    console.error('ESP request failed:', err);
                });
        }


        // button tab
        const buttons = document.querySelectorAll('.tab-button');
        const forms = document.querySelectorAll('.tab-form');

        buttons.forEach((btn, index) => {
            btn.addEventListener('click', () => {
                // Remove active state from all buttons
                buttons.forEach(b => b.classList.remove('active'));
                // Hide all forms
                forms.forEach(f => f.classList.remove('form-active'));
                // Activate clicked button
                btn.classList.add('active');
                // Show the corresponding form
                forms[index].classList.add('form-active');
            });
        });



        const hostname = document.querySelector('input[name="hostname"]');
        const path = document.querySelector('input[name="path"]');
        const port = document.querySelector('input[name="port"]');
        const chuki = document.querySelector('input[name="chuki"]');
        const donvi_chuki = document.querySelector('input[name="donvichuki"]');
        const dosang = document.querySelector('input[name="dosang"]');

        const top = document.querySelector('input[name="cropTop"]');
        const left = document.querySelector('input[name="cropLeft"]');
        const right = document.querySelector('input[name="cropRight"]');
        const bottom = document.querySelector('input[name="cropBottom"]');
        const optionValue = document.querySelector('select[name="options"]');

        const tenKH = document.querySelector('input[name="tenKH"]');
        const SDB = document.querySelector('input[name="SDB"]');
        const PDN = document.querySelector('input[name="PDN"]');


        function checkValueToDefault(component, defaultValue) {
            return component.value != null ? component.value : defaultValue;
        }

        function sendData(event) {
            event.preventDefault(); // Prevent default form submission

            // Collect form data
            let formData = {
                hostname: hostname.value,
                path: path.value,
                port: port.value,
                chuki: chuki.value * donvi_chuki.value,
                dosang: dosang.value,
                cropTop: top.value,
                cropLeft: left.value,
                cropRight: right.value,
                cropBott: bottom.value,
                tenKH: tenKH.value,
                SDB: SDB.value,
                PDN: PDN.value
            };

            fetch("/data", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(formData)
            })
        }

        function handleOnChangeSelect() {
            // console.log("Dropdown changed:", optionValue.value); // Debugging output

            const isDisabled = optionValue.value === "";
            if (isDisabled) {
                top.disabled = !isDisabled;
                left.disabled = !isDisabled;
                right.disabled = !isDisabled;
                bottom.disabled = !isDisabled;
            } else {
                top.disabled = !isDisabled;
                left.disabled = !isDisabled;
                right.disabled = !isDisabled;
                bottom.disabled = !isDisabled;

                if (optionValue.value === "Zenner DN40") {
                    top.value = 222;
                    right.value = 222;
                    left.value = 222;
                    bottom.value = 222;
                } else {
                    top.value = 33;
                    right.value = 33;
                    left.value = 33;
                    bottom.value = 33;
                }
            }
        }

        // Initialize correct state on load
        handleOnChangeSelect();

        checkValueToDefault(hostname, "14.224.158.56");
        checkValueToDefault(path, "some/path");
        checkValueToDefault(port, "8080");
        checkValueToDefault(chuki, "2");
        checkValueToDefault(donvi_chuki, "60");
        checkValueToDefault(dosang, "6");
        checkValueToDefault(top, "6");
        checkValueToDefault(left, "6");
        checkValueToDefault(right, "6");
        checkValueToDefault(bottom, "6");
        checkValueToDefault(tenKH, "6");
        checkValueToDefault(SDB, "6");
        checkValueToDefault(PDN, "6");

        // Add event listener (this ensures it works dynamically)
        optionValue.addEventListener("change", handleOnChangeSelect);
    });
</script>
)rawliteral";

void CheckValueExist(JsonDocument &doc, const String &input, const char *keyName)
{
    if (!input.isEmpty())
        doc[keyName] = input.c_str();
}

void handleStartCheck(AsyncWebServerRequest *request)
{
    String json = R"({
    "network": "đăng kí mạng....done.",
    "serial": "kết nối serial....done.",
    "image": "kiểm tra gửi ảnh....done.",
    "disconnect": "bắt đầu ngắt kết nối trong 5 giây."
  })";
    console.log();
        request->send(200, "application/json", json);
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

    server.on("/check", HTTP_GET, handleStartCheck);
    server.begin();
}

void loop()
{
    dnsServer.processNextRequest();
}
