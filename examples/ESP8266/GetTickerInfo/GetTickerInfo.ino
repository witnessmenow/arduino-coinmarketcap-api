#include <CoinMarketCapApi.h>

/*******************************************************************
 *  An example of getting ticker info from coinmarketcap.com       *
 *                                                                 *
 *  Written by Brian Lough                                         *
 *******************************************************************/


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- Replace the following! ------
char ssid[] = "SSID";       // your network SSID (name)
char password[] = "PASSWORD";  // your network key

WiFiClientSecure client;
CoinMarketCapApi api(client);

// CoinMarketCap's limit is "no more than 10 per minute"
unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_due_time = 0;

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void printTickerData(String ticker) {
  Serial.println("Getting ticker data for " + ticker);
  CMCTickerResponse response = api.GetTickerInfo(ticker, "eur");
  if (response.error == "") {
    Serial.println("Response:");

    Serial.print("Symbol: ");
    Serial.println(response.symbol);
    Serial.print("Price in USD: ");
    Serial.println(response.price_usd);

  } else {
    Serial.print("Error getting data: ");
    Serial.println(response.error);
  }

  // the following fields are available in response
  // struct CMCTickerResponse{
  //   String id;
  //   String name;
  //   String symbol;
  //   int rank;
  //   float price_usd;
  //   float price_btc;
  //   float volume_usd_24h;
  //   float market_cap_usd;
  //   float available_supply;
  //   float total_supply;
  //
  //   float percent_change_1h;
  //   float percent_change_24h;
  //   float percent_change_7d;
  //   float last_updated;
  //
  //   float price_currency;
  //   float volume_currency_24h;
  //   float market_cap_currency;
  //
  //   String error;
  // };
}

void loop() {
  unsigned long timeNow = millis();
  if ((timeNow > api_due_time))  {
    printTickerData("bitcoin");
    printTickerData("ethereum");
    api_due_time = timeNow + api_mtbs;
  }
}
