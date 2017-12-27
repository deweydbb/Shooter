// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueprintTextReader.h"

bool UBlueprintTextReader::FileLoadString(FString FileNameA, FString & SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::GameDir() + FileNameA));
}

bool UBlueprintTextReader::FileSaveString(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::GameDir() + FileNameB));
}

