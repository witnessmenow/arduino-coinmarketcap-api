#include "CoinMarketCapApi.h"

CoinMarketCapApi::CoinMarketCapApi(Client &client)	{
  this->client = &client;
}

CMCTickerResponse responseObject;
String requestedCurrency;

String currentKey;

//**************************************************************************//
// This code is the JSON Parser code written by squix78 as part of his example,
// modified for this application //
// https://github.com/squix78/json-streaming-parser //

class CoinMarketCapListener : public JsonListener {
 public:
  virtual void whitespace(char c);

  virtual void startDocument();

  virtual void key(String key);

  virtual void value(String value);

  virtual void endArray();

  virtual void endObject();

  virtual void endDocument();

  virtual void startArray();

  virtual void startObject();
};

void CoinMarketCapListener::whitespace(char c) {
  // Serial.println("whitespace");
}

void CoinMarketCapListener::startDocument() {
  // Serial.println("start document");
}

void CoinMarketCapListener::key(String key) { currentKey = key; }

void CoinMarketCapListener::value(String value) {
  if (currentKey == F("id")) {
    responseObject.id = value;
  } else if (currentKey == F("name")) {
    responseObject.name = value;
  } else if (currentKey == F("symbol")) {
    responseObject.symbol = value;
  } else if (currentKey == F("rank")) {
    responseObject.rank = value.toInt();
  } else if (currentKey == F("price_usd")) {
    responseObject.price_usd = atof(value.c_str());
  } else if (currentKey == F("price_btc")) {
    responseObject.price_btc = atof(value.c_str());
  } else if (currentKey == F("24h_volume_usd")) {
    responseObject.volume_usd_24h = atof(value.c_str());
  } else if (currentKey == F("market_cap_usd")) {
    responseObject.market_cap_usd = atof(value.c_str());
  } else if (currentKey == F("available_supply")) {
    responseObject.available_supply = atof(value.c_str());
  } else if (currentKey == F("total_supply")) {
    responseObject.total_supply = atof(value.c_str());
  } else if (currentKey == F("percent_change_1h")) {
    responseObject.percent_change_1h = atof(value.c_str());
  } else if (currentKey == F("percent_change_24h")) {
    responseObject.percent_change_24h = atof(value.c_str());
  } else if (currentKey == F("percent_change_7d")) {
    responseObject.percent_change_7d = atof(value.c_str());
  } else if (currentKey == F("last_updated")) {
    responseObject.last_updated = atof(value.c_str());
  } else if (requestedCurrency != "") {
    if (currentKey == "price_" + requestedCurrency) {
      responseObject.price_currency = atof(value.c_str());
    } else if (currentKey == "24h_volume_" + requestedCurrency) {
      responseObject.volume_currency_24h = atof(value.c_str());
    } else if (currentKey == "market_cap_" + requestedCurrency) {
      responseObject.market_cap_currency = atof(value.c_str());
    }
  }
}

void CoinMarketCapListener::endArray() {
}

void CoinMarketCapListener::endObject() {
}

void CoinMarketCapListener::endDocument() {
  // Serial.println("end document. ");
}

void CoinMarketCapListener::startArray() {
}

void CoinMarketCapListener::startObject() {
}

bool CoinMarketCapApi::SendGetToCoinMarketCap(String command) {
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
	long now;
	bool avail;

	if (client->connect(COINMARKETCAP_HOST, Port)) {
		// Serial.println(".... connected to server");
		char c;
		int ch_count=0;
		client->println("GET " + command + " HTTP/1.1");
    client->println("Host: " COINMARKETCAP_HOST);
		client->println(F("User-Agent: arduino/1.0.0"));
		client->println();
		now=millis();
		avail=false;
    client->setTimeout(1200);
    char endOfHeaders[] = "\r\n\r\n";
    bool ok = client->find(endOfHeaders);

    if (!ok) {
      Serial.println("No response or invalid response!");
    }

		return ok;
	}

  return false;
}

CMCTickerResponse CoinMarketCapApi::GetTickerInfo(String coinId, String currency) {
  // https://api.coinmarketcap.com/v1/ticker/bitcoin/
  String command="/v1/ticker/" + coinId + "/";
  if (currency != "") {
    command = command + "?convert=" + currency;
  }

  // Clear old values
  requestedCurrency = currency;
  responseObject = CMCTickerResponse();
  // Serial.println(command);

  if(SendGetToCoinMarketCap(command)) {

    CoinMarketCapListener listener;
    JsonStreamingParser parser;
    parser.setListener(&listener);

    while (client->available()) {
      char c = client->read();
      // parsing code:
      // most of the work happens in the header code
      // at the top of this file
      parser.parse(c);

    }
  } else {
    responseObject.error = "Error with request";
  }

  return responseObject;
}
