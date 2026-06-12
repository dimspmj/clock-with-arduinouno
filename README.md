# clock-with-arduinouno
Jam sederhana yang dilengkapi dengan bar yang akan penuh setiap satu menit dan indikator dari siang hingga malam dan malam ke siang, menggunakan konfigurasi WIB sebagai dasar waktu utama jamnya
# Clock with Arduino Uno

Jam digital sederhana berbasis Arduino Uno. Dibuat buat belajar cara kerja RTC dan tampilan LCD.

---

## Komponen

- Arduino Uno
- RTC DS3231
- LCD 16x2 (I2C)
- Kabel jumper

---

## Wiring

| Modul | Arduino |
|-------|---------|
| RTC SDA | A4 |
| RTC SCL | A5 |
| RTC VCC | 5V |
| RTC GND | GND |
| LCD SDA | A4 |
| LCD SCL | A5 |

*LCD dan RTC berbagi jalur I2C, pastikan alamatnya beda (RTC: 0x68, LCD: 0x27)*

---

## Library yang dipakai

Install lewat Library Manager di Arduino IDE:

- `RTClib` by Adafruit
- `LiquidCrystal_I2C` by Frank de Brabander

---

## Upload

1. Buka file `.ino` di Arduino IDE
2. Pilih board: **Arduino Uno**
3. Pilih port COM yang sesuai
4. Upload

---

## Catatan

Pertama kali upload, waktu RTC akan otomatis tersinkron dengan waktu komputer saat kompilasi. Setelah itu baterai koin CR2032 di modul RTC yang jaga waktunya tetap jalan meski Arduino dimatikan.
