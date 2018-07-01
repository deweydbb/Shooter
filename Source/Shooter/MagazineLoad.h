// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
/**
*
*/
class SHOOTER_API MagazineLoad
{
public:
	MagazineLoad();
	~MagazineLoad();

	int TotalSize;

	int BulletsLeft;

	int FireRate;

	void FireBullet();

	void ReloadMagazine();

	bool CanFire();
};