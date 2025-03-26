
---

## 1. Item

### 1.1 `UDA_CoreItem`

`DA_CoreItem`은 non-editable in runtime 이 기본 원칙.

#### Instance Creation code // 분리하기

현재, 인스턴스를 생성하는 코드가 클래스에 존재.
- [ ]  static 메서드로 분리하기

#### UniqueID 사용처 분석

하나의 아이템 인스턴스가 갖는 고유한 ID 구조체라고 생각되었으나, 그렇지 못한 활용.

##### `A_ItemActor`

- `ResetAllUniqueIDs()` at `AA_ItemActor::BeginPlay()`
	
	`InventoryComponent->ResetAllUniqueIDs();`
	
	왜 UniqueID를 초기화하는 것인지?


#### Trait to Fragment total refactor

@TODO


#### REFACTORING `DefaultContainers`

[**TODO: DEPRECATED codes using DefaultContainers.**]

- `IDA_Equippable` 에서 사용되던 예시의 `DefaultContainers` 관련 코드 TOTAL REFACTOR하기.

- `UDA_CoreItem::GetDefaultConatiners()`

	- ItemContainer를 갖는 Item(e.g. IDA_Equippable)들에 선언된 `TArray<FS_ContainerSettings>`
	
	기존 `ItemContainer` 는 이 TArray 내에 존재하는 것을 `GetDefaultContainers()` 메서드를 통해 받아와서 binding 하는 등, 처리하였음.
	
	- [x] 해당 메서드가 호출되는 부분, 전부 `FindContainersFromItem()` 로 REFACTOR. ✅ 2025-03-21

```cpp title=FindContainersFromItem()

// FL_InventoryFramework.h

static TArray<FS_ContainerSettings>* FindContainersFromItem (FS_InventoryItem& Item);


// FL_InventoryFramework.cpp

TArray<FS_ContainerSettings>* UFL_InventoryFramework::FindContainersFromItem(FS_InventoryItem& Item)  
{
	TArray<FItemContainersFragment*> ItemContainerFragments;
	if (FindFragmentsFromItem<FItemContainersFragment>(Item, ItemContainerFragments))
    {
	    return &ItemContainerFragments[0]->Containers;  
    }
    
	return nullptr;  
}


```




### 1.2 `UAS_GASHelper`


### 1.3 IFP Widgets

#### Inventory Widgets

IFP Demo에서는 AC_Inventory에 설정된 WBP Class를 가져와 `CreateWidget()`, `AddToViewport()` 호출 후, 레퍼런스를 받아 `BindContainerWithWidget()` 호출.

- `BindContainerWithWidget()`
	- Tag를 통한 container binding. `WBP_Container` 위젯(혹은 child class)은 해당 함수를 통해 binding 해주어야만 한다.

이는 Lyra Layout system과 별개로 위젯 instance를 생성하여 viewport에 추가하는 고전적인 방식이므로, 기존 Layout에 맞게 `PushContentToLayerForPlayer` 통해서 적절한 Layer에 넣어줄 수 있는 방식으로 구현해야 한다.

`WBP_IFP_InventoryPanel` 자체는 `GA_ToggleInGameMenu` ability를 통해 보여지는 `WBP_InGameMenu`의 panel들 중, `WBP_InventoryPanel`에 포함되어 있으므로, 추후 필요한 기타 modal layer의 widget들을 위한 system이 될 것.

- 기존 `CreateWidget()`, `AddToViewport()`, `RemoveFromParent()` 노드를 통해 구현되어있는 기타 위젯들의 로직 conversion.
	
	Layer를 사용하기 위해, 새로 그려주는 위젯들을 `CommonActivatableWidget` 으로 reparenting.
	이 위젯들은 보다 상위 layer(`UI.Layer.Modal`)에서 BackHandler로 작동해야 함.
	
	[![[Pasted image 20250313105948.png]]](https://github.com/LichtLights/ProjectP/blob/eff3fc7f5f7e1e91002e756fadf85b6b741183ed/Pasted%20image%2020250313105948.png)
	
	`PushContentToLayerForPlayer` -> `Cast To <MyClass>` -> Custom Initialization function(or event) 순으로 대체.
	
	`PushContentToLayerForPlayer` 노드를 사용하면, 생성된 위젯의 기존 `Construct()` 에서 수행되던 로직을 Exposed on Spawn pin을 사용해 미리 assign 해줄 수 없으므로, 별도의 custom initialization function(or event)를 통해 넘겨준다.
	
	`Construct()` 시점과 차이가 있지만, 위젯을 layer에 push하기 전에 수행하므로, 아직은 문제 없어 보임.
	
	`RemoveFromParent()` 의 경우, Layer에 push된 widget은 제거되지 않음.
	-> `PopContentfromLayer()` 노드를 통해 구현.
	
	- Relevant WBPs
		- `WBP_Inspect` -> Replaced with `WBP_DBItemInspect`
		- `WBP_ContextMenu` -> Replaced with `WBP_DBItemContextMenu`
		
		- `WBP_InventoryItem`
			- OnMouseButtonDown() function, `WBP_Contextmenu`

##### Flow of `PushContentToLayerForPlayer`

CommonGame plugin의 `AsyncAction_PushContentToLayerForPlayer` 비동기 태스크의 static 함수로부터,

```cpp
UAsyncAction_PushContentToLayerForPlayer* UAsyncAction_PushContentToLayerForPlayer::PushContentToLayerForPlayer(APlayerController* InOwningPlayer, TSoftClassPtr<UCommonActivatableWidget> InWidgetClass, FGameplayTag InLayerName, bool bSuspendInputUntilComplete)  
{  
    if (InWidgetClass.IsNull())  
    {       FFrame::KismetExecutionMessage(TEXT("PushContentToLayerForPlayer was passed a null WidgetClass"), ELogVerbosity::Error);  
       return nullptr;  
    }  
    if (UWorld* World = GEngine->GetWorldFromContextObject(InOwningPlayer, EGetWorldErrorMode::LogAndReturnNull))  
    {       UAsyncAction_PushContentToLayerForPlayer* Action = NewObject<UAsyncAction_PushContentToLayerForPlayer>();  
       Action->WidgetClass = InWidgetClass;  
       Action->OwningPlayerPtr = InOwningPlayer;  
       Action->LayerName = InLayerName;  
       Action->bSuspendInputUntilComplete = bSuspendInputUntilComplete;  
       Action->RegisterWithGameInstance(World);  
  
       return Action;  
    }  
    return nullptr;  
}
```

해당 비동기 태스크를 생성하고, `Activate()`

```cpp
void UAsyncAction_PushContentToLayerForPlayer::Activate()  
{  
    if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayout(OwningPlayerPtr.Get()))  
    {       TWeakObjectPtr<UAsyncAction_PushContentToLayerForPlayer> WeakThis = this;  
       StreamingHandle = RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(LayerName, bSuspendInputUntilComplete, WidgetClass, [this, WeakThis](EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget) {  
          if (WeakThis.IsValid())  
          {             switch (State)  
             {                case EAsyncWidgetLayerState::Initialize:  
                   BeforePush.Broadcast(Widget);  
                   break;  
                case EAsyncWidgetLayerState::AfterPush:  
                   AfterPush.Broadcast(Widget);  
                   SetReadyToDestroy();  
                   break;  
                case EAsyncWidgetLayerState::Canceled:  
                   SetReadyToDestroy();  
                   break;  
             }          }          SetReadyToDestroy();  
       });    }    else  
    {  
       SetReadyToDestroy();  
    }}
```

`PrimaryGameLayout`의 template funtion, `PushWidgetToLayerStackAsync` 를 호출.
-> `ActivatableWidgetT* Widget = PushWidgetToLayerStack<ActivatableWidgetT>`
-> 해당 함수에서, `return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);`

-> `return Cast<ActivatableWidgetT>(AddWidgetInternal({}));`
-> `UCommonActivatableWidget* WidgetInstance = GeneratedWidgetsPool.GetOrCreateInstance(ActivatableWidgetClass)`
->
```cpp
/**  
 * Gets an instance of a widget of the given class. * The underlying slate is stored automatically as well, so the returned widget is fully constructed and GetCachedWidget will return a valid SWidget. */
template <typename UserWidgetT = UUserWidget>  
UserWidgetT* GetOrCreateInstance(TSubclassOf<UserWidgetT> WidgetClass)  
{  
    // Just make a normal SObjectWidget, same as would happen in TakeWidget  
    return AddActiveWidgetInternal(WidgetClass,  
       [] (UUserWidget* Widget, TSharedRef<SWidget> Content)  
       {          return SNew(SObjectWidget, Widget)[Content];  
       });}

/** Gets an instance of the widget this factory is for with a custom underlying SObjectWidget type */  
template <typename UserWidgetT = UUserWidget>  
UserWidgetT* GetOrCreateInstance(TSubclassOf<UserWidgetT> WidgetClass, WidgetConstructFunc ConstructWidgetFunc)  
{  
    return AddActiveWidgetInternal(WidgetClass, ConstructWidgetFunc);  
}
```

결과적으로, `AddActiveWidgetInternal()`에서, `CreateWidget`을 통해 instance 생성함.



##### `RefreshBindings` in WBP_IFP_InventoryPanel

-> `WBP_InventoryPanel` 혹은 `WBP_IFP_InventoryPanel`에서 호출 가능한, refresh function을 구현해서, 필요할 때 마다 Container들의 binding을 갱신해줄 함수가 필요하다.

- [x] 하위 Panel들 마다, refresh function들을 구현하여, 상위 menu panel에서 호출할 수 있도록 노출. ✅ 2025-03-01
-> Interface로 구현? (아직은 굳이)

#### Binding a Container in runtime

Backpack, Belt 등 Grid inventory를 갖는 Item들의 runtime container binding.

현재 IFP에서는 runtime binding이 구현되어있지 않다.(멀티플레이어)
Client-only의 경우, 기존 플러그인을 바탕으로 커스텀 구현할 수 있다고 함.

- **Implementation Methods to consider**
	
	- ~~1. Pre-declaration of Containers for each equipment slot~~
		
		`WBP_IFP_InventoryPanel` 에 각 equipment slot에 해당하는 container slot을 생성해두고, 아이템 장착/해제에 따라 container update & visibility change.
		
	- 2. Pre-assigning Attachment Parent Widget class
		
		(기존 `IDA_Equippable의` 구현 방식)
		DA에 Attachment widget class를 assign해 두고, 해당 위젯을 push하는 방식,
		
		각각의 Item마다 고유의 `AttachmentParent` Widget을 구현하고 DA에서 binding해줘야 함.
		-> 어디서, 어떻게 binding 해줄 것인가?
		
	- ~~3. Dynamic creation of WBP_DBContainer widget~~
		
		현재 장착한 equipment들을 tag("IFP.Item.Status.IsEquipped")로 query하여, `ItemContainerFragment` 를 갖는 경우, 동적으로 `WBP_DBContainer` widget을 `CreateWidget()` 통해 생성하여 InventoryPanel 내의 `VB_Inventory` 에 child로 추가한다.
		
		이 경우, Context menu의 "Open"을 통해 따로 여는 경우, 기존 AttachmentParent widget을 활용하던 구현도 따로 해주어야 한다.
		

기존 방식을 따르는, 2번 구현이 가장 적절해 보임.
-> 먼저 IFP에서의 기존 방식, 다른 클래스에서 어떻게 참조되는지 보고, 얼마나 갈아엎을지 결정해야;

- AttachmentParent widget의 DA 내 declaration methods
	
	- 기존 `IDA_Equippable` 방식
		
		IDA 헤더에 선언하고 assign하는 방식.
	
	- AttachmentParent widget class reference를 들고 있는 Fragment를 사용하는 방식
		
		run-time editable 하게 위젯을 바꿀 일이 딱히 있을까?...

- AttachmentParent widget과의 binding procedure breakdown
	
	- `UAC_Inventory::StartComponent()`
		- StartComponent 에서 먼저 DefaultContainer 에 대한 쿼리를 수행.
	
	- `UAC_Inventory::GetItemsChildrenContainers()`
		- 해당 메서드에서 ItemAsset 참조하여 DefaultContainer 를 갖는 경우, 리턴.
		- 애초에 DefaultContainer 자체를 사용하지 않을 것이므로, 수정하거나 날리면 된다.
	
	- `GetDefaultContainers()`
		- `IDA_Equippable` asset에 선언되어 있는 DefaultContainers TArray 에 존재하는 Conatiner들을 가져온다.
		- DefaultContainer 관련 REFACTOR 이후, 재구현하기.

##### Implementation

`WBP_IFP_InventoryPanel` 의 `RefreshBinding` 노드에서, 현재 장비한 아이템을 Tag fragment가 갖는 Tag로 쿼리하여 각각의 장비 슬롯에 해당하는 GridSlot에 `AttachmentWidget` 을 child로 assign.

[![[Pasted image 20250325150305.png]]](https://github.com/LichtLights/ProjectP/blob/627bdd59137515d0e79938e029616e379eb0944c/Pasted%20image%2020250325150305.png)
IsEquipped 태그를 갖는 아이템을 쿼리하고, ItemContainer의 valid check.

[![[Pasted image 20250325144741.png]]](https://github.com/LichtLights/ProjectP/blob/627bdd59137515d0e79938e029616e379eb0944c/Pasted%20image%2020250325144741.png)
tag가 일치하는 경우, 해당 장비 슬롯에 맞게 attachment widget을 assign.


#### Re-creating`WBP_DBItemInspectWidget` bug

- 하나의 WBP_DBInventoryItem 에 대하여, Open 혹은 Inspect 등의 WBP_DBItemInspectWidget 을 Push하는 ContextMenu를 2회 이상 실행했을 경우, 해당 위젯이 clear되지 않는 버그

- PushContentToLayetForPlayer 노드에서 exposed on spawn 변수들을 초기화하여 Construct 시점에 읽어들일 수 없으므로, 현재는 이후에 custom event로 넘겨주고 있음.
	
	RemoveFromParent() 통해서 widget instance를 제거하고 매번 위젯이 호출될 때 마다 새로 `CreateWidget()` 하지 않고 있으므로, 어딘가에서 레퍼런스를 물고 있는 경우 이전 widget instance가 GC되지 않고 그대로 남아있는 것으로 보임.
	
	PushContentToLayerForPlayer 코드를 따라가보면, 결국 CreateWidget() 의 호출부가 존재하는데, 이전 spatical inventory 구현에서도 그랬던 것처럼, widget stack에서 어떤 일이 이루어지고 있는지 blackbox 상태이기 때문에 예측하기 어려움.
	또한, widget stack에서 엔진이 처리하는 일을 손대는 것보다, 밖의 구현에서 해결해야 함.
	
	`RemoveFromParent()` 를 직접 호출하는 것은 어째서인지 instance를 확실히 제거하지 않음.
	
- PopContentFromLayer 또한 위젯 인스턴스를 즉시 GC시키지 않는 것으로 보임.

- A, B 두 아이템의 Inspect 위젯을 번갈아 열게 되면, InventoryItem 자체가 잘못 assign 되어 있는 문제가 있음.
	- A 아이템의 Inspect 호출 후, B 아이템의 Inspect를 호출하면, 여전히 A 아이템의 레퍼런스를 ItemData로 물고 있음?
	- 인스턴스가 남아있더라고, Initialize custom event에서 다시 assign 함에도 불구하고 왜 이전의 레퍼런스를 물고 있는 것인지...???

##### How to Fix

- Method 1
	- Inspect widget의 레퍼런스를 물고 있는 지점을 모두 찾아, Destruct 시점에서 레퍼런스들을 초기화시켜 엔진이 알아서 GC하고, 다시 PushContentToLayerForPlayer 가 호출되는 시점에 해당 위젯 인스턴스가 존재하지 않아 새로운 인스턴스를 생성하게 하는 방법.
	
	-> 이 경우, 일일이 레퍼런스를 초기화시켜야 하므로, straightfoward. 구현하더라도 잘 작동할지 의문임.
	안 그래도 눈에 잘 안 들어오는 BP 노드들이 더 스파게티화 되기 쉽다.

- Method 2
	- 인스턴스가 존재하더라도, 다시 호출되는 시점에, 내부적으로 refresh 하는 로직을 구현하여 기존 widget hierarchy를 유지한 상태에서 위젯 값들만 새로 set 하는 방법.
	
	-> 이 경우, 각각의 위젯에서 초기화하는 로직만 잘 구현하면 됨.
	그러나, 모든 경우에 대한 Inspect widget의 초기화 로직을 따로 구현해야함. 번거로움.

- Method 3
	- CommonGame Lyra plugin 의 Push, Pop Extension을 활용하지 않는 방법.
	
	UPrimayGameLayout 클래스의 stack에 push하는 경우, 하나의 stack layer는 한번에 하나의 activatable widget만이 activation 됨.
	이미 activate 된 widget이 존재할 경우, 새로운 push가 호출되어 stack에 위젯을 추가할 때, 기존 activated 위젯을 deactivate 시키고, 새로운 위젯을 activate.
	activate 된 새 위젯이 pop 되는 경우, 가장 최근에 deactivate 된 위젯을 다시 activate.
	
	하나의 layer에 많은 modal 위젯들을 띄울 수 없는 구현.
	-> CommonGame, CommonUI plugin들을 refactor하여 구현하기에는 엔진 플러그인을 수정해야 하는 리스크가 있으므로, 굳이 손을 대는 것은 좋지 않아 보임.
	
	-> 기존 InGameMenu class에 여러 위젯을 push하여 한번에 handle 가능한 새로운 modal panel 구현하여 처리하기.

-> 가장 안정적이고 단순한 Method 3으로 구현하기.

##### ModalPanel implementation

큰 틀이 되는 `UDBInGameMenu` 클래스는 stack에 push되는 activatable widget.
각각의 `UDBInGameMenu` 는 `UDBGameMenuPanel` 을 BindWidget 하여야 한다.

`UDBGameMenuPanel` 은 `UDBInventoryPanel` 과 같은 child로 구현되고, 특정 위젯 패널이 됨.

GameMenuPanel 이 switch될 때 마다, 해당 패널이 갖는 ModalPanel의 children을 refresh 해줘야 한다. 이는 `UDBGameMenuPanel` 의 `ClearGameMenuModalPanel()` 메서드를 호출하는 것으로, 레퍼런스를 초기화하고 필요 없는 위젯을 GC 시킨다.

```cpp
UFUNCTION(BlueprintCallable)  
void ClearGameMenuModalPanel() const { GameMenuModalPanel->ClearChildren(); }
```



#### 가가방 bug

가방 내 gird에 가방(self) 아이템이 들어가지는 버그.

기존 IFP에서 handle 되던 부분

- Methods
	- Container 내의 Compatibility settings fragment 내 ItemBlacklist 에 self ItemDA를 추가하여 방지.
		-> Straightforward method
		
	- Blocking Tag 활용하여 특정 타입의 아이템을 모두 block.
		-> Straightforward method

일단 가가방에 대한 아이템 기획이 확립되어야 general implementation 고려해볼만 하다.

흠... 그런데 다시 해보니, compatibility setting 설정이 없어도 스스로의 가가방은 되지 않음.

기존의 layout stack에 CommonActivatableWidget 으로 push/pop 하는 과정에서 인스턴스가 GC되지 않고 남아있던 것에 따른 문제였음.


#### `WBP_InventoryItem` DoubleClick handle // Item disappears

현재 `OnMouseDoubleClick()` 호출 시, 해당 container type에 따라 장착하는(`MoveItem()`) 것만 구현되어 있음.

- [ ] `WBP_DBItemContextMenu` 의 특정 `OnPressed` 버튼 노드들을 static하게 만들어서 바로 호출하는 식으로, `InventoryItem` 상호작용을 정리 구현.

- [ ] `GetFirstAvailableTile()` 노드 `SpotFound` 가 true 반환되어 `MoveItem()` 호출되는 버그.
	
	- 이동 가능한 container가 없음에도 true 반환되며, 아이템이 증발함.
		- 현재 장비 중(equipment container에 assign 되어 있는)인 `WBP_DBInventoryItem` 을 더블클릭 시
			- 해당 `WBP_DBInventoryItem` 이 사라짐.
			
		- PocketGrid에 존재하는 `WBP_DBInventoryItem` 을 더블 클릭 시
			- Compatibility setting에 적합한 equipment container가 존재하지 않음에도 해당 `WBP_DBInventoryItem` 이 사라짐.
		
		- Container Fragment를 갖는 Item이 존재하지 않는 경우
			- 더블 클릭 시, 정상적으로 `MoveItem()` 이 호출되지 않음.
	
	- Container Fragment 를 갖는 Backpack item을 장착 중인 경우
		- PocketGrid 의 아이템은 증발하지 않지만, Hands equipment slot의 아이템은 증발하였음.
		
	- IFP_ExampleProejct 에서도 똑같은 버그가 존재... ??

### 1.4 `ItemComponent` total refactor

#### `ItemComponent` breakdown

대부분의 Delegate, 메서드 구현은 블루프린트로 구현되어 있음.

#### ItemTrait

모든 ItemTrait 의 기능을 ItemFragment 로 이전할 것이므로, ItemComponent 또한 필요한 지 봐야함.
replication -> `ItemTrait` & `ItemComponent` 와 `Fragment` 는 replication 방식이 다르다. `Fragment` 는 `Container` 와 동일한 방식(RPC)으로 replicate 된다고 함.
// https://varian.gitbook.io/varian-docs/inventoryframework/inventory-framework/systems/fragments

Trait은 현재 data-only.





### 1.5 Equipment Component

#### IFP `BP_AC_EquipmentHelper`

##### `EquipItem`



##### `Unequipitem`



#### `BPFL_ItemHelpers`


##### LootItem

아이템 루팅 시, 호출되는 static function.
BPFL 노드로만 구현되어 있음 ->
- [x] BPFL 클래스를 따로 만들어 관련 함수들을 C++로 편하게 구현 가능하도록 conversion. ✅ 2025-03-27




#### `IC_Equipment`


#### `IT_Equipment`




## 2. Interaction

### 2.1 Basic interaction system in IFP

IFP의 interaction은 `BPI_Interact` interface의 `PreInteract`, `StartInteract`, `EndInteract` 함수로 단순히 구현되어 있음.

- BP_ItemActor의 구현
	- `PreInteract`
		@TODO
		
	- `StartInteract`
		@TODO
		
	- `EndInteract`
		@TODO


// RPC call을 고려하지 않는다면, 관련 BP component들과 `BPI_Interact`를 굳이 사용할 필요가 없다고 생각?
// 그래도 훗날을 위해 구현해놓는 것이 좋음.

### 2.2 Conversion to Lyra-based interaction system

Lyra의 방법, interaction이 가능해지면 해당 ability를 avatar에 grant.
interaction 불가능한 경우, 해당 ability를 avatar에서 remove.
-> 이 방식으로 다시 구현해보도록 함.
현재, 다른 interaction 파생 ability들 없이, 

`FInteractionOption`에 grant할 ability를 가져와 assign하기.
Methods
	- Interact 파생 ability들을 위한 DA를 참조하기?
	- ConstructionHelper?
	- Interactino과 관련된 Manager class에서 static 함수로 받아오기?

이 경우, `IInteractableTarget`을 상속하는 interaction 가능한 target 액터들이, `DBGameplayAbility_Interact` 범위 내에 들어왔을 때, 해당 Ability의 Target들을 업데이트 하고, 범위 밖으로 나갔을 때, 다시 업데이트 해줘야 함.


#### Lyra interaction option-specific ability implementation

- GA_Collect
	- `GameplayMessageSubsystem`
		
	- `GameplayCueManager`
		
	- `IPickupable`
		Pickup 가능한 actor가 상속하는 interface.
		
		- `AddPickupToInventory()`
			- `FInventoryPickup`
				`FPickupInstance와` `FPickupTemplate의` TArray를 갖는 STRUCT.
				`Pickup->GetPickupInventory()` 호출하여 받아오며, `FPickupTemplate`는 ItemDefinition, `FPickupInstance`는 `ItemInstance` 를 iterate해 받아,
				`InventoryComponent->AddItemDefinition()`
				`InventoryComponent->AddItemInstance()`
				호출한다.
		- 
- GA_Sit
	 GA_Collect와 같은 방식.
	 호출 Tag도 동일함.

-> 같은 호출 태그 `Ability.Interaction.Activated` 로 현재 해당하는 Ability가 호출되는 방식

#### Current DB implementation

-> 각 Interaction ability마다 다른 하위 태그를 EventTag에 assign, 후 `TriggerAbilityFromGameplayEvent` 통해 해당 Ability를 activate하는 방식으로 구현.

기존 구현을 유지하고, 태그로 activation 된 ability에서, IFP 방식으로 handle.

- GA_Ability_Collect
	
	`BP_ItemActor` 에서, `StartInteract` 에 해당하는 부분을 이주, `LootItem` 이 호출되는 부분은 `ItemActor` 에서 `ServerInventoryDataReceived(InventoryComponent)` 내부에 존재.
	
	`LootItem` 이 `true` 를 반환하면, `DestroyActor`



#### `BPI_Interact` RPC Call implementation

`BPI_Interact` base RPC call 구현으로 conversion.

`IInteractableTarget` 인터페이스의,
`virtual void ReadyInteraction() override;`
`virtual void TriggerInteraction() override;`
`virtual void EndInteraction() override;`

- [ ] 상속받아 구현

- `PreInteract`
	
	-> `ReadyInteraction()`
	
	
- `StartInteract`
	
	-> `TriggerInteraction()`
	
	
- `EndInteract`
	
	-> `EndInteraction()`
	



