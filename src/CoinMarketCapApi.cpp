#include "CoinMarketCapApi.h"

CoinMarketCapApi::CoinMarketCapApi(Client &client)	{
  this->client = &client;
}

String CoinMarketCapApi::SendGetToCoinMarketCap(String command) {
  String headers="";
  String body="";
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
	long now;
	bool avail;
	// Connect with google-maps api over ssl
	if (client->connect(COINMARKETCAP_HOST, COINMARKETCAP_SSL_PORT)) {
		// Serial.println(".... connected to server");
		String a="";
		char c;
		int ch_count=0;
		client->println("GET " + command + " HTTP/1.1");
    client->println("Host: " COINMARKETCAP_HOST);
		client->println("User-Agent: arduino/1.0.0");
		client->println("");
		now=millis();
		avail=false;
		while (millis()-now<1500) {
			while (client->available()) {
				char c = client->read();
				//Serial.write(c);

        if(!finishedHeaders){
          if (currentLineIsBlank && c == '\n') {
            finishedHeaders = true;
          }
          else{
            headers = headers + c;

          }
        } else {
          body=body+c;
          ch_count++;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }

				avail=true;
			}
			if (avail) {
				// Serial.println("Body:");
				Serial.println(body);
				// Serial.println("END");
				break;
			}
		}
	}

  return body;
}

CMCTickerResponse CoinMarketCapApi::GetTickerInfo(String coinId, String currency) {
  // https://api.coinmarketcap.com/v1/ticker/bitcoin/
  String command="/v1/ticker/" + coinId + "/";
  if (currency != "") {
    command = command + "?convert=" + currency;
  }

  // Serial.println(command);
  String response = SendGetToCoinMarketCap(command);
  CMCTickerResponse responseObject = CMCTickerResponse();
  DynamicJsonBuffer jsonBuffer;
	JsonArray& root = jsonBuffer.parseArray(response);
  if (root.success()) {
    responseObject.id = root[0]["id"].as<String>();
    responseObject.name = root[0]["name"].as<String>();
    responseObject.symbol = root[0]["symbol"].as<String>();
    responseObject.rank = root[0]["rank"].as<int>();
    responseObject.price_usd = root[0]["price_usd"].as<float>();

    responseObject.price_btc = root[0]["price_btc"].as<float>();
    responseObject.volume_usd_24h = root[0]["24h_volume_usd"].as<float>();
    responseObject.market_cap_usd = root[0]["market_cap_usd"].as<float>();
    responseObject.available_supply = root[0]["available_supply"].as<float>();
    responseObject.total_supply = root[0]["total_supply"].as<float>();

    responseObject.percent_change_1h = root[0]["percent_change_1h"].as<float>();
    responseObject.percent_change_24h = root[0]["percent_change_24h"].as<float>();
    responseObject.percent_change_7d = root[0]["percent_change_7d"].as<float>();
    responseObject.last_updated = root[0]["last_updated"].as<float>();

    currency.toLowerCase();
    responseObject.price_currency = root[0]["price_" + currency].as<float>();
    responseObject.volume_currency_24h = root[0]["volume_" + currency + "_24h"].as<float>();
    responseObject.market_cap_currency = root[0]["market_cap_" + currency].as<float>();
  } else {
    JsonObject& rootObject = jsonBuffer.parseObject(response);
    if (rootObject.containsKey("error")) {
       responseObject.error = rootObject["error"].as<String>();
    } else {
      responseObject.error = "Failed to parse JSON";
    }

    return responseObject;
  }
}
