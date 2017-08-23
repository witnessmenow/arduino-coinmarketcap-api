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

struct CMCTickerResponse{
  String id;
  String name;
  String symbol;
  int rank;
  double price_usd;
  double price_btc;
  double volume_usd_24h;
  double market_cap_usd;
  double available_supply;
  double total_supply;

  double percent_change_1h;
  double percent_change_24h;
  double percent_change_7d;
  double last_updated;

  double price_currency;
  double volume_currency_24h;
  double market_cap_currency;

  String error;
};


class CoinMarketCapApi
{
  public:
    CoinMarketCapApi (Client &client);
    String SendGetToCoinMarketCap(String command);
    CMCTickerResponse GetTickerInfo(String coinId, String currency = "");
    int Port = 443;

  private:
    Client *client;
};

#endif
