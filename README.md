#  SmartParking  
**Proyecto TC2 – Estacionamiento Inteligente con ESP32**

Este proyecto implementa un sistema de estacionamiento inteligente utilizando un ESP32 y comunicación mediante WebSockets.

Nos basamos en este tutorial para guiar el desarrollo (Sigan todo el tutorial, vamos a usar el ambiente similar, nada más que nuestro sensor es de próximidad):  
🔗 [ESP32 WebSocket Server + Sensor](https://randomnerdtutorials.com/esp32-websocket-server-sensor/)

---

##  Requisitos Adicionales que vienen en el tutorial

###  Añadir soporte para ESP32 en Arduino IDE

1. Abre el Arduino IDE.
2. Ve a **Archivo > Preferencias**.
3. En “Gestor de URLs adicionales de tarjetas”, añade:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json


---

###  Librerías necesarias

Asegúrate de instalar las siguientes librerías. Puedes hacerlo desde el **Library Manager** o directamente desde GitHub:

- [Arduino_JSON](https://github.com/arduino-libraries/Arduino_JSON)
- [ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer)
- [AsyncTCP](https://github.com/ESP32Async/AsyncTCP)

---

###  Plugin para subir archivos al ESP32 (LittleFS)

Sigue este tutorial para instalar el plugin necesario para manejar archivos en tu ESP32:  
 [Instalar ESP32 LittleFS en Arduino IDE 2](https://randomnerdtutorials.com/arduino-ide-2-install-esp32-littlefs/)

---

##  Comandos útiles (opcional)

Estas herramientas pueden ayudarte a flashear o borrar la memoria del ESP32 desde terminal:

```bash
pip install esptool
pip install setuptools
python -m esptool
python -m esptool --chip esp32 erase_flash

 
