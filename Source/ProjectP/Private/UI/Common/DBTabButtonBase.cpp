// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/DBTabButtonBase.h"

#include "CommonLazyImage.h"
#include "UI/Common/DBTabListWidgetBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBTabButtonBase)

class UObject;
struct FSlateBrush;

void UDBTabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UDBTabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UDBTabButtonBase::SetTabLabelInfo_Implementation(const FDBTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}

