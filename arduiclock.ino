#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

// --- Konfigurasi Pin OLED SPI (7-Pin) ---
#define SCREEN_WIDTH 128 // Lebar layar OLED, dalam piksel
#define SCREEN_HEIGHT 64 // Tinggi layar OLED, dalam piksel

// Mendefinisikan pin untuk koneksi SPI (menggunakan Hardware SPI Arduino)
#define OLED_MOSI   11 
#define OLED_CLK    13 
#define OLED_DC     9  
#define OLED_CS     10 
#define OLED_RESET  8  

// Membuat objek display OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// --- Konfigurasi RTC ---
RTC_DS3231 rtc; // Ubah menjadi RTC_DS1307 rtc; jika Anda menggunakan modul DS1307

// Array untuk nama hari
char namaHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

void setup() {
  Serial.begin(9600);

  // Inisialisasi Layar OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("Gagal menemukan layar OLED SSD1306"));
    for(;;); 
  }

  display.clearDisplay();
  display.display();

  // Inisialisasi RTC
  if (! rtc.begin()) {
    Serial.println("RTC tidak ditemukan!");
    display.setCursor(0,0);
    display.setTextColor(SSD1306_WHITE);
    display.println("RTC ERROR!");
    display.display();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, mengatur waktu ke waktu kompilasi!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // --- SINKRONISASI WAKTU ---
  // rtc.adjust(DateTime(2026, 4, 17, 21, 55, 0)); 
}

void loop() {
  // Mengambil waktu saat ini dari RTC
  DateTime now = rtc.now();
  int h = now.hour(); // Simpan jam ke dalam variabel 'h' untuk mempermudah

  // Bersihkan layar
  display.clearDisplay();

  // ==========================================
  // 1. HIASAN BINGKAI LAYAR (ROUNDED BORDER)
  // ==========================================
  display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);

  // ==========================================
  // 2. ANIMASI RADAR / SPINNER (KIRI ATAS)
  // ==========================================
  int cx = 14, cy = 18, r = 6;
  display.drawCircle(cx, cy, r, SSD1306_WHITE); // Lingkaran luar
  float angle = (millis() / 10) % 360;          // Kecepatan putaran
  float rad = angle * PI / 180.0;
  int px = cx + (r-1) * cos(rad);
  int py = cy + (r-1) * sin(rad);
  display.drawLine(cx, cy, px, py, SSD1306_WHITE); // Garis berputar

  // ==========================================
  // 3. INDIKATOR SIKLUS HARI (KANAN ATAS)
  // ==========================================
  int iconX = 114, iconY = 18;
  
  if (h >= 6 && h < 10) {
    // FASE 1: Pagi (Matahari Terbit)
    display.drawCircle(iconX, iconY + 2, 4, SSD1306_WHITE); // Badan matahari
    display.drawLine(iconX - 8, iconY + 2, iconX + 8, iconY + 2, SSD1306_WHITE); // Garis horizon
    // Sinar atas
    display.drawLine(iconX, iconY - 4, iconX, iconY - 2, SSD1306_WHITE);
    display.drawLine(iconX - 4, iconY - 1, iconX - 3, iconY, SSD1306_WHITE);
    display.drawLine(iconX + 4, iconY - 1, iconX + 3, iconY, SSD1306_WHITE);
    // Hapus bagian bawah horizon agar terlihat terbit
    display.fillRect(iconX - 8, iconY + 3, 17, 5, SSD1306_BLACK); 
    
  } else if (h >= 10 && h < 15) {
    // FASE 2: Siang (Matahari Terik)
    display.drawCircle(iconX, iconY, 3, SSD1306_WHITE);
    display.drawLine(iconX, iconY-6, iconX, iconY+6, SSD1306_WHITE); 
    display.drawLine(iconX-6, iconY, iconX+6, iconY, SSD1306_WHITE); 
    display.drawLine(iconX-4, iconY-4, iconX+4, iconY+4, SSD1306_WHITE); 
    display.drawLine(iconX-4, iconY+4, iconX+4, iconY-4, SSD1306_WHITE); 
    display.fillCircle(iconX, iconY, 2, SSD1306_BLACK); 
    
  } else if (h >= 15 && h < 18) {
    // FASE 3: Sore (Matahari Terbenam)
    display.drawCircle(iconX, iconY + 3, 4, SSD1306_WHITE); // Posisi lebih rendah
    display.drawLine(iconX - 8, iconY + 4, iconX + 8, iconY + 4, SSD1306_WHITE); // Garis horizon
    display.fillRect(iconX - 8, iconY + 5, 17, 5, SSD1306_BLACK); // Tenggelam
    
  } else if (h >= 18 && h < 22) {
    // FASE 4: Malam Awal (Bulan Purnama / Cembung & Bintang)
    display.fillCircle(iconX, iconY, 4, SSD1306_WHITE); // Bulan utuh
    display.fillCircle(iconX - 1, iconY - 1, 1, SSD1306_BLACK); // Kawah 1
    display.fillCircle(iconX + 2, iconY + 1, 1, SSD1306_BLACK); // Kawah 2
    display.drawPixel(iconX - 6, iconY - 4, SSD1306_WHITE); // Bintang
    display.drawPixel(iconX + 5, iconY - 5, SSD1306_WHITE); // Bintang
    
  } else {
    // FASE 5: Dini Hari / Larut Malam (Bulan Sabit & Bintang)
    display.fillCircle(iconX, iconY, 5, SSD1306_WHITE);
    display.fillCircle(iconX - 2, iconY - 2, 4, SSD1306_BLACK); // Potong untuk efek sabit
    display.drawPixel(iconX + 6, iconY - 2, SSD1306_WHITE); // Bintang 1
    display.drawPixel(iconX - 5, iconY + 4, SSD1306_WHITE); // Bintang 2
  }

  // ==========================================
  // 4. MENAMPILKAN JAM & TITIK DUA BERKEDIP
  // ==========================================
  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(26, 12); 
  
  // Jam
  if (now.hour() < 10) display.print('0');
  display.print(now.hour(), DEC);
  
  // Animasi titik dua berkedip
  if (now.second() % 2 == 0) {
    display.print(':');
  } else {
    display.print(' ');
  }
  
  // Menit
  if (now.minute() < 10) display.print('0');
  display.print(now.minute(), DEC);
  
  // Detik
  display.setTextSize(1);             
  display.setCursor(88, 19);
  if (now.second() < 10) display.print('0');
  display.print(now.second(), DEC);

  // ==========================================
  // 5. MENAMPILKAN TANGGAL
  // ==========================================
  display.setTextSize(1);             
  display.setCursor(12, 34);          
  
  // Format Tanggal
  display.print(namaHari[now.dayOfTheWeek()]);
  display.print(", ");
  if (now.day() < 10) display.print('0');
  display.print(now.day(), DEC);
  display.print('/');
  if (now.month() < 10) display.print('0');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.year(), DEC);

  // ==========================================
  // 6. ANIMASI PROGRESS BAR SEGMENTASI DI BAWAH
  // ==========================================
  // Kotak luar
  display.drawRect(13, 50, 104, 7, SSD1306_WHITE); 
  
  // Menghitung jumlah segmen (0 hingga 20 kotak)
  int numSegments = map(now.second(), 0, 59, 0, 20); 
  
  // Menggambar kotak-kotak kecil sejumlah detik yang berlalu
  for(int i = 0; i < numSegments; i++) {
    display.fillRect(15 + (i * 5), 52, 4, 3, SSD1306_WHITE); 
  }

  // Dorong semua hiasan ke layar
  display.display();

  // Jeda kecil
  delay(100); 
}