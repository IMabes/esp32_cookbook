# ESP32 OTA (Kablosuz Firmware Güncelleme)

## OTA nedir?

OTA (Over-The-Air), ESP32'ye yeni kodu **USB kablosuz, WiFi üzerinden** yükleme yöntemidir. Normalde her kod değişikliğinde board'u bilgisayara USB ile bağlaman gerekir; OTA kurulduktan sonra board aynı WiFi ağındaysa kabloya hiç dokunmadan güncelleyebilirsin.

## Nasıl çalışır?

1. Board açılınca WiFi'ye bağlanır.
2. `ArduinoOTA.begin()` ile ağ üzerinden gelecek bir yükleme isteğini dinlemeye başlar.
3. Bilgisayardan `pio run -e ota -t upload` komutu çalıştırıldığında, yeni firmware WiFi üzerinden board'a gönderilir ve board kendini günceller.

## Kurulum ve ilk çalıştırma

1. **İlk yükleme mutlaka USB ile yapılmalı** (board'da henüz OTA kodu çalışmıyor, WiFi'den ulaşılamaz):
   ```
   pio run -e usb -t upload
   ```

2. Serial Monitor'ü aç (`pio device monitor` veya PlatformIO sidebar), board'un WiFi'ye bağlandığı IP adresini not al:
   ```
   Bağlandı! IP: 192.168.x.x
   ```

3. `platformio.ini` dosyasındaki `env:ota` bölümünde `upload_port` değerini bu IP ile güncelle.

## Sonraki güncellemeler (kablosuz)

Koda değişiklik yaptıktan sonra:
```
pio run -e ota -t upload
```

Terminalde bir ilerleme yüzdesi göreceksin — bu, yeni kodun WiFi üzerinden board'a gönderildiğinin işareti.

## Notlar

- OTA şifresi (`--auth`) kodda ve `platformio.ini`'de birebir aynı olmalı, yoksa yükleme reddedilir.
- OTA modunda Serial Monitor çalışmaz — kodun çalışıp çalışmadığını görmek için tekrar USB'ye bağlanman gerekir.
- Board'un IP adresi zaman zaman değişebilir (router yeniden IP dağıtırsa); yükleme başarısız olursa Serial Monitor'den güncel IP'yi tekrar kontrol et.