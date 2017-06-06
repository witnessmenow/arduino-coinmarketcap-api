/*
Copyright (c) 2017 Brian Lough. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/


#ifndef CoinMarketCapApi_h
#define CoinMarketCapApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define COINMARKETCAP_HOST "api.coinmarketcap.com"
#define COINMARKETCAP_SSL_PORT 443

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


class CoinMarketCapApi
{
  public:
    CoinMarketCapApi (Client &client);
    String SendGetToCoinMarketCap(String command);
    CMCTickerResponse GetTickerInfo(String coinId, String currency = "");

  private:
    Client *client;
    bool checkForOkResponse(String response);
};

#endif
