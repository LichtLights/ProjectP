// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_IFPSystem/Items/DA_DBCoreItem.h"

#include "Core/Widgets/W_AttachmentParent.h"

TSubclassOf<UW_AttachmentParent> UDA_DBCoreItem::GetAttachmentWidgetClass()
{
	if (IsValid(AttachmentWidget))
	{
		return AttachmentWidget;
	}
	
	return Super::GetAttachmentWidgetClass();
}
