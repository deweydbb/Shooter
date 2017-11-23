// Fill out your copyright notice in the Description page of Project Settings.

#include "MagazineLoad.h"
#include "Shooter.h"

MagazineLoad::MagazineLoad()
{
	TotalSize = 30;
}

MagazineLoad::~MagazineLoad()
{
}

void MagazineLoad::FireBullet()
{
	BulletsLeft -= 1;
}

void MagazineLoad::ReloadMagazine()
{
	BulletsLeft = TotalSize;
}

bool MagazineLoad::CanFire() {
	if (BulletsLeft > 0) {
		return true;
	}
	else {
		return false;
	}
}