# arduino-coinmarketcap-api
A wrapper around the [CoinMarketCap.com](http://coinmarketcap.com/) API for Arduino (supports ESP8266).

The CoinMarketCap API can be used to look information on a wide range of crypto currency coins, included Bitcoin and Ethereum (check out the full list on their site).

## Usage

The simplest way to see how it works is the check out [the example that comes with the library](https://github.com/witnessmenow/arduino-coinmarketcap-api/tree/master/examples/ESP8266/GetTickerInfo).

### GetTickerInfo:
A wrapper around the single ticker endpoint supporting the convert parameter (currency)

Code is as follows:
```
CMCTickerResponse response = api.GetTickerInfo(ticker, "eur");
```

Ticker unfortunately is not the symbol for some reason. Go to [CoinMarketCap.com](http://coinmarketcap.com/) and select the coin you would like to check, the ticker name makes up the last part of the URL. e.g: https://coinmarketcap.com/currencies/bitcoin/ , "bitcoin" is the ticker value

Currency is optional, so you can pass only ticker if you want.
Check out the currency drop down on [CoinMarketCap.com](http://coinmarketcap.com/) to get available values

The return object has the following values:
```
struct CMCTickerResponse{
  String id;
  String name;
  String symbol;
  int rank;
  float price_usd;
  float price_btc;
  float volume_usd_24h;
  float market_cap_usd;
  float available_supply;
  float total_supply;

  float percent_change_1h;
  float percent_change_24h;
  float percent_change_7d;
  float last_updated;

  float price_currency;
  float volume_currency_24h;
  float market_cap_currency;

  String error;
};
```
