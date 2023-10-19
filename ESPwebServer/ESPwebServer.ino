#include <WiFi.h>

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

const char* ssid     = "ESPintar";
const char* password = "espintar123";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;

String header; // Variable para guardar el HTTP request

String estadoSalida = "off";

const int salida = 2;

//------------------------CODIGO HTML------------------------------
String pagina = R"====(<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>ESPintar</title>
        <style>
            body {
                display: grid;
                margin: 0;
                padding: 0;
                font-family: system-ui, -apple-system, BlinkMacSystemFont,
                    Segoe UI, Roboto, Oxygen, Ubuntu, Cantarell, Open Sans,
                    Helvetica Neue, sans-serif;
                background-color: #0c0c0c;
                color: #fff;
            }
            header {
                display: grid;
                grid-template-columns: 1fr 3fr;
                background: #0c0c0c;
                color: #fff;
                height: 10svh;
                font-weight: 800;
            }
            .title {
                display: flex;
                justify-content: center;
                grid-column: 2;
            }
            #content {
                display: grid;
                justify-content: center;
                align-items: center;
                height: 90svh;
                grid-template-columns: 1fr 3fr;
            }
            @media only screen and (max-width: 768px) {
                header {
                    grid-template-columns: 1fr;
                }
                .title {
                    grid-column: 1;
                }
                #content {
                    grid-template-columns: 1fr;
                }
                #grid-container {
                    grid-row: 1;
                }
            }
            :root {
                --grid-size: 16;
            }
            #grid-container {
                display: flex;
                height: 100%;
                align-items: center;
                justify-content: center;
                background-color: #222;
                border-radius: 5px;
            }
            #grid {
                justify-self: center;
                display: grid;
                grid-template-columns: repeat(var(--grid-size), 1fr);
                grid-template-rows: repeat(var(--grid-size), 1fr);
                width: clamp(320px, 50vw, 550px);
                height: clamp(320px, 50vw, 550px);
                background-color: #0c0c0c;
                padding: 5px;
                gap: 5px;
            }
            .grid-cell {
                border: 1px solid #ccc;
                background-color: #000;
                border-radius: 10%;
                -webkit-user-select: none;
                user-select: none;
            }
            .grid-cell:hover {
                opacity: 0.8;
                background-color: var(--selected-color);
            }
            #controls-container {
                background-color: #0c0c0c;
                display: flex;
                flex-direction: column;
                min-width: 200px;
                width: 80%;
                justify-self: center;
                padding: 5px;
                justify-content: center;
                gap: 10px;
            }
            #controls {
                display: grid;
                border-radius: 5px;
                grid-template-columns: repeat(auto-fit, minmax(50px, 1fr));
            }
            .color-cell {
                aspect-ratio: 1;
            }
            .color-cell:hover {
                border: solid 2px #ffffff;
            }
            .controls-btn-container {
                display: grid;
                width: 100%;
                grid-template-columns: repeat(2, 1fr);
                gap: 10px;
            }
            .controls-btn {
                display: flex;
                width: 5rem;
                justify-self: center;
                justify-content: center;
                width: 90%;
            }
            #load-input {
                display: none;
            }
        </style>
    </head>
    <body>
        <header>
            <h1 class="title">ESPintar</h1>
        </header>

        <div id="content">
            <div id="controls-container">
                <div id="controls"></div>
                <div class="controls-btn-container">
                    <button id="eraser" class="controls-btn">Borrador</button>
                    <button id="clear" class="controls-btn">Limpiar</button>
                    <input
                        type="color"
                        id="color-picker"
                        class="controls-btn"
                    />
                </div>
                <div class="saveload-container">
                    <div id="save-btn">Save Grid</div>
                    <label for="load-input">Upload Grid</label>
                    <input
                        type="file"
                        id="load-input"
                        accept=".json"
                        name="load-input"
                    />
                </div>
            </div>
            <div id="grid-container">
                <div id="grid"></div>
            </div>
        </div>
        <script>
            (function () {
                const t = document.createElement("link").relList;
                if (t && t.supports && t.supports("modulepreload")) return;
                for (const r of document.querySelectorAll(
                    'link[rel="modulepreload"]'
                ))
                    n(r);
                new MutationObserver((r) => {
                    for (const c of r)
                        if (c.type === "childList")
                            for (const a of c.addedNodes)
                                a.tagName === "LINK" &&
                                    a.rel === "modulepreload" &&
                                    n(a);
                }).observe(document, { childList: !0, subtree: !0 });
                function o(r) {
                    const c = {};
                    return (
                        r.integrity && (c.integrity = r.integrity),
                        r.referrerPolicy &&
                            (c.referrerPolicy = r.referrerPolicy),
                        r.crossOrigin === "use-credentials"
                            ? (c.credentials = "include")
                            : r.crossOrigin === "anonymous"
                            ? (c.credentials = "omit")
                            : (c.credentials = "same-origin"),
                        c
                    );
                }
                function n(r) {
                    if (r.ep) return;
                    r.ep = !0;
                    const c = o(r);
                    fetch(r.href, c);
                }
            })();
            const l = 16,
                m = document.documentElement,
                s = document.getElementById("grid"),
                y = document.getElementById("controls");
            let d = "";
            (() => {
                for (let e = 0; e < l * l; e++) {
                    const t = document.createElement("div");
                    t.classList.add("grid-cell"),
                        (t.id = `cell-${e}`),
                        s.appendChild(t),
                        t.addEventListener("mousedown", () => f(t)),
                        t.addEventListener("mouseover", (o) => {
                            o.buttons === 1 && f(t);
                        });
                }
            })();
            const f = (e) => {
                    switch (d) {
                        case "ERASER":
                            e.style.backgroundColor = "";
                        default:
                            e.style.backgroundColor = d;
                    }
                },
                g = (e) => {
                    const t = document.elementFromPoint(e.clientX, e.clientY);
                    t.classList.contains("grid-cell") && f(t);
                };
            s.addEventListener("touchstart", (e) => {
                (isDrawing = !0), g(e.touches[0]);
            });
            s.addEventListener("touchmove", (e) => {
                e.touches.length >= 2 ||
                    (e.preventDefault(), isDrawing && g(e.touches[0]));
            });
            s.addEventListener("touchend", () => {
                isDrawing = !1;
            });
            const u = {
                0: "#ffffff",
                1: "#232035",
                2: "#44293c",
                3: "#ff0000",
                4: "#df7027",
                5: "#d8a067",
                6: "#efc39b",
                7: "#fbf337",
                8: "#98e450",
                9: "#6abf31",
                10: "#37946e",
                11: "#4b682e",
                12: "#524b24",
                13: "#323c39",
                14: "#3e3f75",
                15: "#316083",
                16: "#5a6ee0",
                17: "#639afe",
                18: "#5ecde5",
                19: "#cadafc",
                20: "#dddddd",
                21: "#9badb7",
                22: "#857f87",
                23: "#686a6b",
            };
            (() => {
                for (let e = 0; e < 24; e++) {
                    const t = document.createElement("div");
                    t.classList.add("color-cell"),
                        (t.id = `color-${e}`),
                        (t.style.backgroundColor = u[e]),
                        t.addEventListener("mousedown", () => {
                            (d = `${u[e]}`),
                                m.style.setProperty(
                                    "--selected-color",
                                    `${u[e]}`
                                );
                        }),
                        y.appendChild(t);
                }
            })();
            const E = document.getElementById("eraser"),
                b = document.getElementById("clear"),
                h = document.getElementById("color-picker"),
                p = () => {
                    s.childNodes.forEach((e) => {
                        e.style.backgroundColor = "";
                    });
                };
            E.addEventListener("mousedown", () => {
                (d = "ERASER"),
                    m.style.setProperty("--selected-color", "#000000");
            });
            b.addEventListener("mousedown", p);
            h.addEventListener("input", (e) => {
                (d = e.target.value),
                    m.style.setProperty("--selected-color", d);
            });
            const v = () => {
                    const e = [];
                    let t = 0;
                    for (i = 0; i < l; i++)
                        for (j = 0; j < l; j++) {
                            let o = s.childNodes[t].style.backgroundColor;
                            o && o != "rgb(0, 0, 0)" && e.push([[i, j], o]),
                                t++;
                        }
                    return JSON.stringify(e);
                },
                L = () => {
                    const e = v(),
                        t = new Blob([e], { type: "application/json" }),
                        o = URL.createObjectURL(t),
                        n = document.createElement("a");
                    (n.href = o),
                        (n.download = "gridData.json"),
                        (n.style.display = "none"),
                        document.body.appendChild(n),
                        n.click(),
                        document.body.removeChild(n),
                        URL.revokeObjectURL(o);
                },
                w = document.getElementById("save-btn");
            w.addEventListener("click", L);
            const C = (e) => {
                    const t = JSON.parse(e);
                    console.log(t),
                        p(),
                        t.forEach((o) => {
                            let n = o[0][0] * l + o[0][1];
                            s.childNodes[n].style.backgroundColor = o[1];
                        });
                },
                O = (e) => {
                    const t = new FileReader();
                    (t.onload = (o) => {
                        const n = o.target.result;
                        C(n);
                    }),
                        t.readAsText(e);
                },
                D = document.getElementById("load-input");
            D.addEventListener("change", (e) => {
                const t = e.target.files[0];
                t && O(t);
            });
        </script>
    </body>
</html>)====";


//---------------------------SETUP--------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("");
  
  pinMode(salida, OUTPUT); 
  digitalWrite(salida, LOW);

  // Conexión WIFI
  WiFi.begin(ssid);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      //IPAddress ip(192,168,1,180); 
      //IPAddress gateway(192,168,1,1); 
      //IPAddress subnet(255,255,255,0); 
      //WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      server.begin(); // iniciamos el servidor
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

//----------------------------LOOP----------------------------------

void loop(){
  WiFiClient client = server.available();   // Escucha a los clientes entrantes

  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // enciende y apaga el GPIO
//            if (header.indexOf("GET /on") >= 0) {
//              Serial.println("GPIO on");
//              estadoSalida = "on";
//              digitalWrite(salida, HIGH);
//            } else if (header.indexOf("GET /off") >= 0) {
//              Serial.println("GPIO off");
//              estadoSalida = "off";
//              digitalWrite(salida, LOW);
//            }
            
            // Muestra la página web
            client.println(pagina);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
