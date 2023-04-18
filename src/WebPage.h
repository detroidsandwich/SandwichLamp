#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <string>

const byte MAX_SLIDER_VALUE = 255;
const byte FACTOR = 16;

//['apple', 'banana', 'cherry', 'orange', 'lemon'];\n";
String addMode(String *modes, int size)
{
    String modesArray = "[";
    for (size_t i = 0; i < size; i++)
    {
        modesArray += "'";
        modesArray += modes[i];
        modesArray += "'";
        if (i < size - 1)
        {
            modesArray += ",";
        }
    }
    modesArray += "];\n";

    return modesArray;
}

static String createWebPage(String *modes, int8 modeSize, struct LedData *ledData)
{
    // htmlPage += "const modes = " + addMode(modes, modeSize);
    // htmlPage += "const modeCount = " + String(modeSize) + ";\n";
    // htmlPage += "const mode = " + String(ledData->numberEffect) + ";\n";
    // htmlPage += "const brightness =" + String(ledData->brightness / FACTOR) + ";\n";
    // htmlPage += "const speed = " + String(ledData->speed / FACTOR) + ";\n";
    // htmlPage += "const scale = " + String(ledData->scale / FACTOR) + ";\n";
    String htmlPage = "";
    htmlPage += "<!DOCTYPE html>\n";
    htmlPage += "<html>\n";
    htmlPage += "<head>\n";
    htmlPage += "<title>Hello World</title>\n";
    htmlPage += "<style>\n";
    htmlPage += "body {\n";
    htmlPage += "text-align: center;\n";
    htmlPage += "font-family: Arial, sans-serif;\n";
    htmlPage += "flex-direction: column;\n";
    htmlPage += "display: flex;\n";
    htmlPage += "flex-direction: column;\n";
    htmlPage += "align-items: center;\n";
    htmlPage += "justify-content: center;\n";
    htmlPage += "}\n";
    htmlPage += ".card {\n";
    htmlPage += "display: inline-block;\n";
    htmlPage += "padding: 20px;\n";
    htmlPage += "border-radius: 10px;\n";
    htmlPage += "background-color: #f0f0f0;\n";
    htmlPage += "box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.2);\n";
    htmlPage += "margin-bottom: 16px;\n";
    htmlPage += "}\n";
    htmlPage += ".btn {\n";
    htmlPage += "margin: 10px;\n";
    htmlPage += "cursor: pointer;\n";
    htmlPage += "border-radius: 50%;\n";
    htmlPage += "width: 50px;\n";
    htmlPage += "height: 50px;\n";
    htmlPage += "font-size: 24px;\n";
    htmlPage += "display: inline-flex;\n";
    htmlPage += "justify-content: center;\n";
    htmlPage += "align-items: center;\n";
    htmlPage += "background-color: #999;\n";
    htmlPage += "color: #fff;\n";
    htmlPage += "border: none;\n";
    htmlPage += "outline: none;\n";
    htmlPage += "box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.2);\n";
    htmlPage += "}\n";
    htmlPage += ".btn:hover {\n";
    htmlPage += "background-color: #777;\n";
    htmlPage += "}\n";
    htmlPage += "#slider {\n";
    htmlPage += "vertical-align: middle;\n";
    htmlPage += "display: inline-block;\n";
    htmlPage += "}\n";
    htmlPage += "#output {\n";
    htmlPage += "font-size: 24px;\n";
    htmlPage += "display: inline-block;\n";
    htmlPage += "margin: 0 10px;\n";
    htmlPage += "}\n";
    htmlPage += ".label {\n";
    htmlPage += "font-size: 18px;\n";
    htmlPage += "margin-bottom: 10px;\n";
    htmlPage += "}\n";
    htmlPage += "</style>\n";
    htmlPage += "<script>\n";
    htmlPage += "const modes = " + addMode(modes, modeSize);
    htmlPage += "const modeCount = " + String(modeSize) + ";\n";
    htmlPage += "const mode = " + String(ledData->numberEffect) + ";\n";
    htmlPage += "const brightness =" + String(ledData->brightness / FACTOR) + ";\n";
    htmlPage += "const speed = " + String(ledData->speed / FACTOR) + ";\n";
    htmlPage += "const scale = " + String(ledData->scale / FACTOR) + ";\n";
    htmlPage += "const maxSlider = " + String(MAX_SLIDER_VALUE / FACTOR) + ";\n";
    // htmlPage += "const modes = ['apple', 'banana', 'cherry', 'orange', 'lemon'];\n";
    // htmlPage += "const modeCount = 5;\n";
    // htmlPage += "const mode = 2;\n";
    // htmlPage += "const brightness = 2;\n";
    // htmlPage += "const speed = 2;\n";
    // htmlPage += "const scale = 2;\n";
    // htmlPage += "const maxSlider = 16;\n";
    // htmlPage += "// const factor = 16;\n";
    // htmlPage += "// const maxValue = 255; // 0..255;\n";
    htmlPage += "window.onload = function () {\n";
    htmlPage += "setupModeSlider(document.getElementById(\"mode\"), mode);\n";
    htmlPage += "setupSlider(document.getElementById(\"brightness\"), brightness);\n";
    htmlPage += "setupSlider(document.getElementById(\"speed\"), speed);\n";
    htmlPage += "setupSlider(document.getElementById(\"scale\"), scale);\n";
    htmlPage += "};\n";
    htmlPage += "function setupModeSlider(element, value) {\n";
    htmlPage += "var slider = element.querySelector(\"#slider\");\n";
    htmlPage += "var output = element.querySelector(\"#output\");\n";
    htmlPage += "slider.max = modeCount - 1;\n";
    htmlPage += "output.innerHTML = modes[value];\n";
    htmlPage += "slider.value = value;\n";
    htmlPage += "var prevSliderValue = slider.value;\n";
    htmlPage += "slider.oninput = function () {\n";
    htmlPage += "if(prevSliderValue != this.value){\n";
    htmlPage += "updateModeData(element, this.value);\n";
    htmlPage += "prevSliderValue = this.value;\n";
    htmlPage += "}\n";
    htmlPage += "}\n";
    htmlPage += "var btnIncrement = element.querySelector(\"#btnIncrement\");\n";
    htmlPage += "var btnDecrement = element.querySelector(\"#btnDecrement\");\n";
    htmlPage += "btnIncrement.onclick = function () {\n";
    htmlPage += "if (parseInt(slider.value) < parseInt(slider.max)) {\n";
    htmlPage += "updateModeData(element, parseInt(slider.value) + 1);\n";
    htmlPage += "}\n";
    htmlPage += "}\n";
    htmlPage += "btnDecrement.onclick = function () {\n";
    htmlPage += "if (parseInt(slider.value) > parseInt(slider.min)) {\n";
    htmlPage += "updateModeData(element, parseInt(slider.value) - 1);\n";
    htmlPage += "}\n";
    htmlPage += "}\n";
    htmlPage += "};\n";
    htmlPage += "function updateModeData(element, value) {\n";
    htmlPage += "var xhttp = new XMLHttpRequest();\n";
    htmlPage += "var slider = element.querySelector(\"#slider\");\n";
    htmlPage += "var output = element.querySelector(\"#output\");\n";
    htmlPage += "xhttp.onreadystatechange = function () {\n";
    htmlPage += "if (this.readyState == 4 && this.status == 200) {\n";
    htmlPage += "console.log(this.responseText);\n";
    htmlPage += "output.innerHTML = modes[value]\n";
    htmlPage += "slider.value = value\n";
    htmlPage += "}\n";
    htmlPage += "}\n";
    htmlPage += "xhttp.open(\"GET\", \"/\" + element.id + \"?value=\" + value, true);\n";
    htmlPage += "xhttp.send();\n";
    htmlPage += "// output.innerHTML = modes[value]\n";
    htmlPage += "// slider.value = value\n";
    htmlPage += "};\n";
    htmlPage += "function setupSlider(element, initialValue) {\n";
    htmlPage += "var slider = element.querySelector(\"#slider\");\n";
    htmlPage += "var output = element.querySelector(\"#output\");\n";
    htmlPage += "output.innerHTML = initialValue;\n";
    htmlPage += "slider.max = maxSlider;\n";
    htmlPage += "slider.value = initialValue;\n";
    htmlPage += "var prevSliderValue = slider.value;\n";
    htmlPage += "slider.oninput = function () {\n";
    htmlPage += "if(prevSliderValue != this.value){\n";
    htmlPage += "updateSliderData(element, this.value);\n";
    htmlPage += "prevSliderValue = this.value;\n";
    htmlPage += "}\n";
    htmlPage += "}\n";
    htmlPage += "var btnIncrement = element.querySelector(\"#btnIncrement\");\n";
    htmlPage += "var btnDecrement = element.querySelector(\"#btnDecrement\");\n";
    htmlPage += "btnIncrement.onclick = function () {\n";
    htmlPage += "if (parseInt(slider.value) < parseInt(slider.max)) {\n";
    htmlPage += "updateSliderData(element, parseInt(slider.value) + 1);\n";
    htmlPage += "}\n";
    htmlPage += "}\n";
    htmlPage += "btnDecrement.onclick = function () {\n";
    htmlPage += "if (parseInt(slider.value) > parseInt(slider.min)) {\n";
    htmlPage += "updateSliderData(element, parseInt(slider.value) - 1);\n";
    htmlPage += "}\n";
    htmlPage += "}\n";
    htmlPage += "};\n";
    htmlPage += "function updateSliderData(element, value) {\n";
    htmlPage += "var xhttp = new XMLHttpRequest();\n";
    htmlPage += "var slider = element.querySelector(\"#slider\");\n";
    htmlPage += "var output = element.querySelector(\"#output\");\n";
    htmlPage += "xhttp.onreadystatechange = function () {\n";
    htmlPage += "if (this.readyState == 4 && this.status == 200) {\n";
    htmlPage += "console.log(this.responseText);\n";
    htmlPage += "output.innerHTML = value\n";
    htmlPage += "slider.value = value\n";
    htmlPage += "}\n";
    htmlPage += "}\n";
    htmlPage += "xhttp.open(\"GET\", \"/\" + element.id + \"?value=\" + value, true);\n";
    htmlPage += "xhttp.send();\n";
    htmlPage += "// output.innerHTML = value\n";
    htmlPage += "// slider.value = value\n";
    htmlPage += "};\n";
    htmlPage += "// function correct\n";
    htmlPage += "</script>\n";
    htmlPage += "</head>\n";
    htmlPage += "<body>\n";
    htmlPage += "<h1>Hello World!</h1>\n";
    htmlPage += "<p>Welcome to my website.</p>\n";
    htmlPage += "<div class=\"card\" id=\"mode\">\n";
    htmlPage += "<div class=\"label\">Mode:<span class=\"label\" id=\"output\">0</span></div>\n";
    htmlPage += "<button id=\"btnDecrement\" class=\"btn\">-</button>\n";
    htmlPage += "<input type=\"range\" id=\"slider\" min=\"0\">\n";
    htmlPage += "<button id=\"btnIncrement\" class=\"btn\">+</button>\n";
    htmlPage += "</div>\n";
    htmlPage += "<div class=\"card\" id=\"brightness\">\n";
    htmlPage += "<div class=\"label\">Brightness:<span id=\"output\">50</span></div>\n";
    htmlPage += "<button id=\"btnDecrement\" class=\"btn\">-</button>\n";
    htmlPage += "<input type=\"range\" id=\"slider\" min=\"0\">\n";
    htmlPage += "<button id=\"btnIncrement\" class=\"btn\">+</button>\n";
    htmlPage += "</div>\n";
    htmlPage += "<div class=\"card\" id=\"speed\">\n";
    htmlPage += "<div class=\"label\">Speed:<span id=\"output\">50</span></div>\n";
    htmlPage += "<button id=\"btnDecrement\" class=\"btn\">-</button>\n";
    htmlPage += "<input type=\"range\" id=\"slider\" min=\"0\">\n";
    htmlPage += "<button id=\"btnIncrement\" class=\"btn\">+</button>\n";
    htmlPage += "</div>\n";
    htmlPage += "<div class=\"card\" id=\"scale\">\n";
    htmlPage += "<div class=\"label\">Scale:<span id=\"output\">50</span></div>\n";
    htmlPage += "<button id=\"btnDecrement\" class=\"btn\">-</button>\n";
    htmlPage += "<input type=\"range\" id=\"slider\" min=\"0\">\n";
    htmlPage += "<button id=\"btnIncrement\" class=\"btn\">+</button>\n";
    htmlPage += "</div>\n";
    htmlPage += "</body>\n";
    htmlPage += "</html>\n";

    return htmlPage;
};

#endif