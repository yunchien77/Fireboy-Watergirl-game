# 2025 OOPL Final Report

## I. 組別資訊

- **組別**：22
- **組員**：B812110004 葉芸茜、112590450 李馥亘
- **復刻遊戲**：Fireboy and Watergirl

## II. 專案簡介

### A. 遊戲簡介

本專案是以雙人合作的解謎遊戲《Fireboy and Watergirl: Forest Adventure》為基礎，運用物件導向程式設計（OOP）的概念，重新開發並實作各種角色特性、機關互動及關卡挑戰。遊戲強調合作與策略，結合了物理碰撞偵測、事件觸發與介面管理。

### B. 組別分工

- **葉芸茜**：UI 物件及地圖製作、角色系統開發、角色與機關的碰撞邏輯、機關系統、關卡系統、網格系統碰撞、遊戲流程控制、報告撰寫、重構程式碼

- **李馥亘**：UI 物件及地圖製作、機關系統、UI 系統與網格系統、角色與機關的碰撞邏輯、遊戲流程控制、關卡系統、報告撰寫、重構程式碼

## III. 遊戲介紹

### A. 遊戲規則

#### 移動控制
- 🔥 Fireboy
  * ```←``` Move left
  * ```→``` Move right
  * ```↑``` Jump

- 💧 Watergirl
  * ```A``` Move left
  * ```D``` Move right
  * ```W``` Jump

#### 角色特性
- Fireboy 不怕火，會被水淹死
- Watergirl 不怕水，會被火燒死
- 兩人都怕綠色毒液池 ☠

#### 合作玩法
- 兩個角色必須同時到達各自出口門才能過關
- 收集紅寶石（Fireboy）和藍寶石（Watergirl）可獲得分數
- 完成關卡後可解鎖下一關

#### 互動機關
- **按鈕、拉桿**：啟動門或平台
- **門**：需要觸發才能打開
- **平台**：上下移動，可搭乘
- **箱子**：可推動當墊腳石
- **風扇**：吹起角色到高處

#### 暫停功能
- 暫停按鈕位於畫面右上角
- 暫停選單選項：
  - ```Resume```：繼續遊戲
  - ```Retry```：重新開始當前關卡
  - ```End```：返回關卡選擇畫面

#### 開發者模式（作弊模式）
- 按 ```I``` 鍵可切換無盡模式，角色不會死亡，適合測試或探索

### B. 專案連結

[Fireboy & Watergirl GitHub](https://github.com/yunchien77/Fireboy-Watergirl-game/tree/main)

### C. 遊戲畫面

- 主選單
![alt text](url)
- 關卡選擇
![alt text](url)
- Level 1
![alt text](url)
- Level 2
![alt text](url)
- Level 3
![alt text](url)
- Level 4
![alt text](url)
- Level 5
![alt text](url)
- 通關畫面
![alt text](url)

## IV. 程式設計

### A. 程式架構

本專案以 OOP 概念為核心，將各遊戲元素以模組化設計，整體架構分為以下層次：

#### 項目根目錄結構

```
FIREBOY-WATERGIRL-GAME/                                    
├── PTSD/        
├── Resources/  
├── include/     
├── src/               
├── .gitignore         
├── CMakeLists.txt          
├── files.cmake             
├── FinalReport.md          
├── README.md                  
└── LICENSE              
```

#### include/ 目錄結構

```
include/
├── Character/
│   ├── Character.hpp
│   ├── CharacterComponent.hpp
│   ├── CharacterState.hpp
│   ├── Fireboy.hpp
│   └── Watergirl.hpp
├── Game/
│   ├── CollisionManager.hpp
│   ├── GameMechanicsManager.hpp
│   └── InputHandler.hpp
├── Interface/
│   └── ITriggerable.hpp
├── Level/
│   ├── Level.hpp
│   ├── Level1.hpp
│   ├── Level2.hpp
│   ├── Level3.hpp
│   ├── Level4.hpp
│   ├── Level5.hpp
│   └── LevelFactory.hpp
├── Mechanism/
│   ├── Box.hpp
│   ├── Button.hpp
│   ├── Door.hpp
│   ├── Fan.hpp
│   ├── Gate.hpp
│   ├── Gem.hpp
│   ├── Lever.hpp
│   ├── LiquidTrap.hpp
│   ├── MechanismBase.hpp
│   └── Platform.hpp
└── Object/
    ├── BackgroundImage.hpp
    ├── Color.hpp
    ├── GridSystem.hpp
    └── Option.hpp
```

#### src/ 目錄結構

```
src/
├── Character/
│   ├── Character.cpp
│   ├── CharacterCollisionComponent.cpp
│   ├── CharacterMovementComponent.cpp
│   ├── CharacterPhysicsComponent.cpp
│   ├── Fireboy.cpp
│   └── Watergirl.cpp
├── Game/
│   └── GameManager/
│       ├── GameMechanicsManager.cpp
│       ├── InputHandler.cpp
│       ├── AppGamePause.cpp
│       ├── AppGamePlay.cpp
│       ├── AppGameWin.cpp
│       ├── AppLevelSelect.cpp
│       ├── AppLoading.cpp
│       ├── AppReset.cpp
│       └── AppStart.cpp
├── Level/
│   ├── Level.cpp
│   ├── Level1.cpp
│   ├── Level2.cpp
│   ├── Level3.cpp
│   ├── Level4.cpp
│   ├── Level5.cpp
│   ├── LevelFactory.cpp
│   └── LoadLevel.cpp
├── Mechanism/
│   ├── Box.cpp
│   ├── Button.cpp
│   ├── Door.cpp
│   ├── Fan.cpp
│   ├── Gate.cpp
│   ├── Gem.cpp
│   ├── Lever.cpp
│   ├── LiquidTrap.cpp
│   ├── MechanismBase.cpp
│   └── Platform.cpp
├── Object/
│   └── GridSystem.cpp
├── App.cpp
└── main.cpp
```


### B. 程式技術

#### 1. 封裝（Encapsulation）

| 類別名稱 | 封裝內容 |
|---------|---------|
| Character / Fireboy / Watergirl | 將角色的位置、速度、跳躍狀態、是否受風影響等資訊設為私有。透過函式如 `Move()`、`Jump()`、`ApplyExternalForce()`、`IsGrounded()`、`GetPosition()` 等與外部互動。 |
| Box | 封裝箱子的移動與重力邏輯，透過 `ApplyGravity()` 控制落下、`CheckCollisionWithTerrain()` 檢查地形碰撞、`OnCollisionWithCharacter()` 實作角色推箱子邏輯等。 |
| GridSystem | 封裝地圖格子資料 `m_Grid`，提供座標轉換 `CellToGamePosition()` / `GameToCellPosition()`，查詢格子類型 `GetCell()`，以及地形互動邏輯 `CanMoveOn()` / `CanStandOn()` 等。 |
| Platform | 封裝動畫移動與碰撞檢查邏輯，透過 `UpdateAnimation()` 處理移動、`IsCharacterOn()` 判斷人物是否站立在平台上。 |
| Button / Lever | 封裝觸發狀態與與目標機關的連動邏輯，僅透過 `Update()` 判斷是否被角色觸發，並呼叫註冊的 ITriggerable 物件。 |
| Gate | 封裝開關狀態、動畫、初始位置等資料，外部僅能透過 `OnTriggered()` / `OnReleased()` 操作開關，無需知道門的動畫與顯示邏輯。 |
| App | 封裝整體遊戲流程狀態，如遊戲階段控制、關卡載入、背景音樂播放、按鈕初始化等行為均集中在 App 類中處理，對外只需調用相關函式（如 `LoadLevelGrid()`、`ResetGame()` 等。 |
| Door | 封裝開門與關門動畫狀態與更新邏輯。對外僅提供 `SetOpen()`、`Update()`、`Respawn()`。 |

#### 2. 繼承（Inheritance）

| 父類別 | 子類別 | 說明 |
|--------|--------|------|
| Character | Fireboy / Watergirl | 繼承基本角色操作邏輯（移動、跳躍、重力、動畫等），並可針對各自屬性（如火、水）進行差異化處理。 |
| MechanismBase | Box / Gem / Gate / Lever / Button / LiquidTrap / Platform / Fan | 這些都是「機關類」的子類別，繼承共同屬性如 `SetPosition()`、`SetDrawable()`、`GetColor()`、`Respawn()` 等，並各自實作特殊邏輯（如按鈕觸發、平台移動、風力作用等）。 |
| Level | Level1 ~ Level5 | 每個關卡皆繼承自 Level 抽象類別，實作對應的 `Initialize()`，配置角色初始位置、寶石、機關分佈等。 |

#### 3. 多型（Polymorphism）

| 使用方式 | 多型設計 | 說明 |
|----------|----------|------|
| ITriggerable 介面 | Gate、Platform 等實作該介面 | 可被 Button / Lever 統一呼叫 `OnTriggered()` 與 `OnReleased()`，即使實際執行邏輯不同（例如打開門或移動平台），皆可透過相同介面互動。 |
| LevelFactory::CreateLevel() | 回傳 `std::unique_ptr<Level>` | 利用父類別指標操作 Level1 ~ Level5 等子類，支援動態多型。呼叫 `level->Initialize()` 時會執行對應子類版本。 |
| Character 指標操作 | Fireboy / Watergirl 所有角色控制相關程式碼皆以 Character 指標處理 | 讓碰撞檢查、動畫更新、事件觸發等不需關心是 Fireboy 還是 Watergirl，統一使用 Character 指標。 |
| std::shared_ptr<MechanismBase> 容器 | 機關皆可統一加入畫面、處理更新邏輯 | 利用基底類別指標容納所有機關類別，配合 `SetDrawable()`、`Update()` 執行共通操作。 |

#### 4. 介面（Interface）

| 介面名稱 | 方法 | 實作類別 |
|----------|------|----------|
| ITriggerable | OnTriggered()、OnReleased() | Gate, Platform |

#### 5. 組合（Composition）

| 擁有者 | 被組合物件 | 說明 |
|--------|------------|------|
| App | Level, Character, Door, Renderer, Option 等 | App 管理遊戲流程，組合多個元件並統一更新與操作。 |
| Level | GridSystem, Gem, Gate, Button, Lever, Platform, Box, Fan 等集合 | 每個關卡組合多種機關物件，集中初始化與管理。 |

#### 6. 抽象（Abstraction）

| 類別 / 介面 | 抽象功能 | 說明 |
|-------------|----------|------|
| Character | 抽象角色基本能力（移動、跳躍、碰撞、動畫） | 讓 Fireboy 與 Watergirl 只需實作差異部分，其餘皆封裝於 Character 類中。 |
| MechanismBase | 抽象所有機關的共同行為（位置、初始狀態、圖片設定等） | 所有機關皆只需實作 `Update()`、`OnTriggered()`、`OnReleased()` 等方法即可互動，其他畫面邏輯與基本屬性由基底類別提供。 |
| ITriggerable 介面 | 抽象出可被觸發機關的操作介面 | Button / Lever 不需知道觸發對象是門還是平台，只要實作了 ITriggerable 的物件皆可控制。 |
| Level | 抽象出關卡共用的行為與初始化流程 | Level1 ~ Level5 只需關注具體機關配置，其他如角色載入、地圖加載等流程統一封裝在 Level。 |

## V. 結語

### A. 問題與解決方法

#### 問題1：素材與網格系統不對齊
原版遊戲的素材格子並不是統一的大小，而是歪歪的，非常不規整。這與我們採用的網格系統設計理念相衝突，直接使用會導致視覺效果不佳，且處理與地形的碰撞時會出戲，影響遊戲體驗。因此我們手動進行圖片後製處理，重新調整素材的尺寸和對齊方式，確保所有地形素材盡量符合統一的網格尺寸。

#### 問題2：角色中心點碰撞判定不準確及頻繁撞牆
最初的碰撞檢測是從角色的正中間進行判斷，導致角色在地面上會有「懸浮」的視覺效果，看起來不自然。後來將碰撞檢測點調整到角色的中間底部，讓角色能夠正確地「站立」在地面上。而在調整碰撞點後，角色在移動時經常卡在牆壁上，移動體驗不順暢，因此改採多點碰撞檢測系統，在角色的左右上下設定多個關鍵點來負責偵測碰撞：

- **左右兩側**：設定側邊檢測點，避免角色卡在垂直牆面
- **頂部**：設定頭部檢測點，防止角色穿過天花板
- **底部**：設定腳部檢測點，確保重力和站立判定正確

#### 問題3：重力系統與地面判定衝突
角色的重力判定範圍過大，導致在某些情況下角色無法正確落地或出現異常的物理行為。通過以下方法來優化碰撞及重力處理：

- 將重力相關的底部判斷範圍縮小至僅限腳部區域
- 精確調整重力強度和跳躍力度的平衡

通過這些問題的解決過程，我們體會到遊戲開發中最基礎會遇到的一些問題，真的是沒寫過就不知道，包含怎麼設計架構，該怎麼實作，或是一些維護的方式。每一個看似簡單的功能背後都可能隱藏著複雜的技術挑戰。雖然過程遇到一些很難解決的 bug，但最終成功解決這些問題讓我們對程式設計有了更深入的理解。

### B. 自評

| 項次 | 項目 | 完成 |
|------|------|------|
| 1 | 完成專案權限改為 public | ✓ |
| 2 | 具有 debug mode 的功能 | ✓ |
| 3 | 解決專案上所有 Memory Leak 的問題 | ✓ |
| 4 | 報告中沒有任何錯字，以及沒有任何一項遺漏 | ✓ |
| 5 | 報告至少保持基本的美感，人類可讀 | ✓ |
| 6 | 乖乖按進度寫完 | ✓ |
| 7 | 與成員間合作愉快 :D | ✓ |
| 8 | 遊戲復刻程度高達80% | ✓ |
| 9 | 程式架構支援未來擴充 | ✓ |
| 10 | 維持良好的心情迎接每一次上課 🌝 | ✓ |
| 11 | 全勤 🎉 | ✓ |

### C. 心得

透過這次的 OOPL 專案，我們將上學期學習到的物件導向程式設計運用在本次復刻《Fireboy and Watergirl》的專案。其中，我們使用了封裝、繼承、多型、介面等概念，並在完成遊戲基本功能後進行了大規模的程式碼重構，使整體架構變得更加清晰且易於維護，也便於以後遊戲的擴充。

在復刻遊戲的過程中，設計角色碰撞的時候花費了比預期多的時間去修改，不只需要檢測和地形之間的碰撞，還必須考慮角色與各種機關互動時的特殊情況。發現平常看似簡單的遊戲，其實是有許多細節需要去反覆修改及檢查的。

在團隊合作過程中，我們也學習到了如何分工合作開發大型專案。透過良好的程式架構設計和 GitHub 版本控制的使用，即使是不同人在不同時間寫出的程式碼也可以順利地整合。過程中我們也掌握了解決程式碼衝突的技巧，學會設定正確的 Git 配置讓每個人的貢獻都能正確顯示在專案歷史中。這些實務經驗讓我們對軟體開發的協作流程有了更深入的認識，真是有趣的一堂實作課呢！

這次專案不僅讓我們對 C++ 語言和物件導向程式設計有了更加深入和實用的理解，更重要的是體驗到了完整軟體開發的全過程。經過一學期的合作與不斷討論，當我們看到最終成品能夠順利運行，兩個角色在關卡中通關的畫面，真的很有成就感！

### D. 貢獻比例

| 成員 | 貢獻度百分比 |
|------|---------|
| 葉芸茜 | 50% |
| 李馥亘 | 50% |