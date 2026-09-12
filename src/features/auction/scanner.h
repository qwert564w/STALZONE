#pragma once
#include <string>
#include <vector>
struct AuctionLot { int id; std::string name; int qty; long long price; long long market; int discount; long long total; };
struct ScannerConfig { bool enabled = false; bool onlyDeals = true; bool hideArtifacts = false; float minDiscount = 20.0f; int sortMode = 0; };
namespace AuctionScanner { extern ScannerConfig cfg; extern std::vector<AuctionLot> lots; void Scan(); void RenderMenu(); void BuyLot(int id); }