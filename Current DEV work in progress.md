

# TODOs

### TODOS

#### Consider this;

- [x] 🔼 non-ability PlayerCharacter actions -> NativeAction으로 convert(Lyra) ✅ 2025-02-02
- [ ] ~~🔼 Interaciton sphere overlap -> weird collision debugging~~
- [x] Interaction 비동기 task(Lyra)로 구현할 것인지? ✅ 2025-03-23
---
#### Must do


##### Done
- [x] Stackable 관련 구현 ✅ 2024-12-04
- [x] Item Rotation 구현 ✅ 2024-12-03
- [x] Stackable Item Split 기능 구현 ✅ 2024-12-06
	- Case: 비어있는 GridCell에 Drop
	- Case: 이미 존재하는 같은 종류의 GridItem에 Drop
	- [x] Drop 시, 수량 split하는 widget 구현 ✅ 2024-12-06
	- [x] Split 팝업의 경우, 닫기 버튼을 비활성화하기 ✅ 2024-12-06 SafeClose 통해 해결
	- [x] PlayerController에서 De-referencing하기. 하지 않으면 HandleCanceled가 call되어 템 복사됨. ✅ 2024-12-06
	- [x] SafeClose 가상함수 구현 ✅ 2024-12-05
- [x] `PopUpContent_Inspection` 구현 ✅ 2024-12-14
- [x] `HoverContent_ItemInfo` 구현 ✅ 2024-12-14
---
#### Bug to be fixed

- [ ] ~~ItemHover 시 Viewport clamp 되지 않는 버그~~
- [ ] ~~가방 Grid에서 ContextOption 중 Discard 시, MEMORY VIOLATION~~

##### Fixed

- [x] SetCellHighlight VIOLATION 버그 ✅ 2024-12-03
	- GridCell의 레퍼런스를 배열로 `InventoryGridWidget`에서 물어 GC 되지 않게 함으로써 해결 (X)
	- 인벤토리를 켜둔 상태로 조작없이 시간이 좀 지나면 위젯이 GC되는지 여전히 VIOLATION 발생
		- GC되는 것 = `OwningGrid->HighlightedCells`
		- EXCEPTION_ACCESS_VIOLATION
			- at the line `HighlightedCell->SetCellHighlight(EGridCellHighlight::None);`
			- nullptr check만 했기 때문에 발생한 문제.
```cpp title:IsValid()
if (!OwningGrid->HighlightedCells.IsEmpty())  
{  
	for (UGridCell* HighlightedCell : OwningGrid->HighlightedCells)  
	{
	    if (!IsValid(HighlightedCell))  
	    {
		    UE_LOG(LogTemp, Warning, TEXT("HighlightedCell is null at UGridCell::NativeOnDragOver()"));  
		    continue;
	    }
		HighlightedCell->SetCellHighlight(EGridCellHighlight::None);  
    }
}
```

- [ ] 
---
### Weekly TODOS
#### 25.01
##### ~25.01.19 // **Prolonged** ~25.02.02
- Lyra system conversion
	- [x] Input system conversion ✅ 2025-02-02
	- [x] HUD Widget conversion ✅ 2025-02-02
		- [x] Stack widget ✅ 2025-02-02
---

##### ~25.02.09
- [x] ADBPlayerCharacter::Interact() GameplayAbility로 재구현 ✅ 2025-02-18
	- 일단 GA로 옮기고 나서 생각하기
	- Lyra Interaction system 뜯어보기... < TAKELONG
	
- [x] Clear&Remove DEPRECATED classes ✅ 2025-02-18
	- [x] AMainHUD ✅ 2025-02-18
	- [x] AMainHUDWidget ✅ 2025-02-18
	- [x] HUDWidgets/ ✅ 2025-02-18
	- [x] SubWidgets/StatusProgressBar ✅ 2025-02-03
	- [x] DebugMenuWidget ✅ 2025-02-02
		- DBDebugMenu should be used. 콘솔창 등, 혹은 DEV 태그 따로 적용 가능한 시스템 구현 고려하기.
---

##### ~25.02.23
- [x] IFP 플러그인 뜯어보기 ✅ 2025-02-23


---

##### ~25.03.06 // **Prolonged** ~ 25.03.16
- **IFP 플러그인**
	- Trait & Components -> DEPRECATED // 굳이?
	- Trait은 현재 definition처럼 한 아이템의 모든 인스턴스가 같은 값을 공유, 변경하면 전체 적용됨. ->
		- [ ] 관련 구현들을 Fragments system으로 이전하기.
			- [x] Pricing ✅ 2025-02-27
			- [ ] CustomShape
			- [ ] GAS System integration
		
	- Widget integration
		- [x] BP_DBInventoryComponent->WidgetClass 베이스가 되는 위젯 구현 ✅ 2025-03-10
			- WBP_InventoryPanel 내부 패널에 들어갈 위젯. 인벤토리 컴포넌트에 assign.
			
		- `BindContainerWithWidget` 통해서 IFP Panel의 Container에 binding 해줘야 위젯이 정상적으로 초기화 됨.
			-> 매번 위젯을 다시 생성해서 그려줘야 하는가?
			-> Lyra의 구현과 대치되는 부분. 어떻게?
			-> Panel의 위젯을 refresh 해줘야 한다면, Panel에 존재하는 위젯의 레퍼런스, 혹은 slot의 레퍼런스를 가져올 수 있다면, 필요할 때마다 refresh하는 함수를 호출하여, 현재 구현과 통합 가능.
			- [x] `BindContainerWithWidget` 통해서 정상적인 UI initiailization 구현하기. ✅ 2025-03-06
			
		- Demo 폴더 위젯 handle.
			- [x] Demo 폴더 하위의 모든 위젯의 dependency 제거하고, Core 위젯은 직접 수정해서 사용하기. ✅ 2025-03-13
			- 레퍼런스를 물고 있거나 특히, `CreateWidget` 의 경우, 직접 다른 DB 클래스 위젯으로 바꿔주어야 함.
			
		- WBP_Inspect
			- [x] 다른 BP들에서 참조하고 있는 WBP_Inspect를 WBP_DBItemInspect로 바꾸기. ✅ 2025-03-13
			
		- WBP_ContextMenu
			- [x] WBP_DBItemContextMenu로 conversion. ✅ 2025-03-13
			- ContextMenu button widget
				- [x] WBP_DynamicButton -> WBP_DBContextMenuButton으로 replace. ✅ 2025-03-13
			
		- WBP_InventoryItem
			- [x] WBP_ContextMenu 위젯 `CreateWidget` 통해 `AddToViewport` 하는 블루프린트 로직들을 modal layer에 `PushContentToLayerForPlayer` 로 넣어주는 로직으로 변경하기. ✅ 2025-03-10
				- [x] WBP_ContextMenu를 CommonActivatableWidget으로 reparenting. ✅ 2025-03-10
				
			- WBP_DBInventoryItem
			
		- WBP_InventoryItemBackground
			- [x] WBP_DBInventoryItemBackground로 replace. ✅ 2025-03-13
			
		- WBP_Highlight
			- 기존 IFP Core 폴더의 WBP_Highlight 그대로 사용하기.
			
---

##### ~25.03.23
###### SUMMARY

- [x] CommonUI -> InputMode // `LyraActivatableWidget` class 에서 수정하여 마우스, 위젯 interactable 여부 수정. ✅ 2025-03-16

- [x] `AC_Inventory` -> `ResetAllUniqueIDs()` 문제의 함수 확인하기 ✅ 2025-03-21

###### TODOs

- UI Widget DELEGATE to `GameplayMessageSubsystem`
	- [x] PopUp widget 재구현하고, ✅ 2025-03-16
	- [ ] ~~PopUp Widget - PlayerController DELEGATE Binding conversion to GameplayMessageSubsystem~~
	- 블루프린트 노드에서 Listen for Gameplay Messages 노드
	- 커스텀 GameplayTag 채널 통해 listen
	- Payload는 PopUpContextInfo 등등?
	- [x] -> IFP 플러그인의 ContextMenu 기반으로 Layer.Modal에 Push하는 방식으로 일단 구현됨. ✅ 2025-03-16
	
- [ ] ~~ADBPlayerController::OnRotateItem() GameplayAbility로 재구현?~~
	- UIAbility. 기존의 `CurrentInventoryDragDrop` 포인터 변수를 참조하는 방식은 유지해야 할 듯?
	- 혹은 현재 활성화된 DragDropOp 인스턴스를 참조할 수 있는 다른 방법 찾아보기?
	-> IFP 방식으로 IA를 직접 바인딩 된 상태.
	
- **IFP 플러그인**
	- Trait -> DEPRECATED // 굳이?
	- Trait은 현재 definition처럼 한 아이템의 모든 인스턴스가 같은 값을 공유, 변경하면 전체 적용됨. 
	- 대부분은 `FTags` Fragment를 통해 GameplayTag 기반으로 구현 가능해보임.
	- `FTags`의 child struct들을 활용해서 나누어 구현? -> Fragment는 getter by struct.
		
		- [ ] 관련 구현들을 Fragments system으로 이전하기.
			- [x] Pricing ✅ 2025-02-27
			- [ ] CustomShape
			- [ ] GAS System integration
		
	- DA_CoreItem의 전반적인 구현 방식 조정
		- [x] ItemStructFragment, ItemAssetFragment REFACTOR ✅ 2025-03-21
			- Data-driven -> Data-only & Read-only DA, 하나로 통일.
			- Runtime writable data -> Tag 기반으로 세이브 로직에 맞게 변환 가능성 고려하여 Refactor.
		
	- Widget integration
		- [x] WBP_Highlight, 아이템을 container 위, 드래그 호버 시, 남아있는 문제 ✅ 2025-03-17
			- `WBP_Drag` 위젯에서 `Destruct` 시, InventoryComponent를 casting 하는 부분에서, `BP_DBInventoryComponent` 가 아닌, 기존의 `BP_AC_Inventory` 로 casting 하는 노드에 의해서 `FadeItem` 함수가 call 되지 않았던 문제.
		
	- Blueprint only class REFACTORING
		- [x] Refactor `BPI_IFP_Helpers` to C++ based inteface. ✅ 2025-03-21
			- [x] LootItem ✅ 2025-03-17
			- [x] SellItem ✅ 2025-03-17
	
---


##### ~25.03.30
###### SUMMARY

GDC 인디 애프터파티. 간접적이지만 느낄 수 있었던 분위기 // 인디페스티벌에서 많은 사람들에게 피드백 받기

###### TODOs
- WeaponMastery
	- [ ] WeaponMasteryPanel UI
		- [ ] UI Node
	- [ ] WeaponMasteryComponent
		- [ ] @TODO
	
- DBGameplayAbility_Interact
	- DBInteractionPromptWidget
		- [ ] IsInteractable() 반환해서 Indicator visibiltiy set up... -> how?
	
	- [ ] IFP 플러그인의 BPI_Interact 방식의 구현을 기존 IInteractableTarget 인터페이스에 통합
		- Detailed in [[Planning to refactor plugin]]  -> **2. Interaction section**
	
	
- **IFP 플러그인**
	- Trait -> DEPRECATED // 굳이?
	- Trait은 현재 definition처럼 한 아이템의 모든 인스턴스가 같은 값을 공유, 변경하면 전체 적용됨. 
	- 대부분은 `FTags` Fragment를 통해 GameplayTag 기반으로 구현 가능해보임.
	- `FTags`의 child struct들을 활용해서 나누어 구현? -> Fragment는 getter by struct.
		
		- [ ] 관련 구현들을 Fragments system으로 이전하기.
			- [x] Pricing ✅ 2025-02-27
			- [ ] CustomShape
			- [ ] GAS System integration
		
	- ItemComponent -> Should use?
		
	- Equipment Component 구현
		- [ ] Custom Equip 기능 구현하기.
			- IC_Equipment 관련 코드 이전하기.
			- IT_Equipment 관련 코드 이전하기.
			
		- [ ] BP_EquipmentHelper
			- 관련 코드 이전하기.
	
- UE_5.4 to UE_5.5
	- Lyra 코드들 5.5에 맞게 대조, 수정
		
		- [ ] AbilitySystem
		
		- Lyra plugin 코드들
		
		- [ ] AsyncMixin
		- [ ] CommonGame
		- [ ] CommonLoadingScreen
		- [ ] CommonUser
		- [ ] GameplayMessageRouter
		- [ ] GameSubtitles
		- [ ] GameSettings
		- [ ] ModularGameplayActors
		- [ ] UIExtension
		
---
