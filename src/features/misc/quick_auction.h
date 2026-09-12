#pragma once
struct QuickAuctionConfig { bool enabled = false; int openKey = VK_F5; int closeKey = VK_ESCAPE; int delay = 120; };
namespace QuickAuction { extern QuickAuctionConfig cfg; void Update(); void RenderMenu(); }