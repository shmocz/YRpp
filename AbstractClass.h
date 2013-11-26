#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <Interfaces.h>
#include <GeneralDefinitions.h>
#include <GeneralStructures.h>
#include <ArrayClasses.h>

//forward declarations
class HouseClass;
class Checksummer;

//--- OwnedTiberiumStruct - holds info about how much of each tiberium type is held.
struct OwnedTiberiumStruct
{
	static const size_t Size = 4;

	float GetAmount(int index)
		{ JMP_THIS(0x6C9680); }

	float GetTotalAmount()
		{ JMP_THIS(0x6C9650); }

	float AddAmount(float amount, int index)
		{ JMP_THIS(0x6C9690); }

	float RemoveAmount(float amount, int index)
		{ JMP_THIS(0x6C96B0); }

	int GetTotalValue()
		{ JMP_THIS(0x6C9600); }

	float Tiberium1;
	float Tiberium2;
	float Tiberium3;
	float Tiberium4;
};
//---

//The AbstractClass is the base class of all game objects.
class AbstractClass : public IPersistStream, public IRTTITypeInfo, public INoticeSink, public INoticeSource
{
public:
	enum {AbsID = abs_Abstract};

	static DynamicVectorClass<AbstractClass *>* Array0;

	//static
	const char* GetClassName() const
	{
		return AbstractClass::GetClassName(this->WhatAmI());
	}

	static const char* GetClassName(eAbstractType abs)
	{
		NamedValue* ass;
		for(ass = (NamedValue*)0x816EE0; ass < (NamedValue*)0x817130; ass++)
		{
			if(ass->Value == abs) {
				return ass->Name;
			}
		}

		return nullptr;
	}

	//IUnknown
	virtual HRESULT __stdcall QueryInterface(REFIID iid, void** ppvObject) R0;
	virtual ULONG __stdcall AddRef() R0;
	virtual ULONG __stdcall Release() R0;

	//IPersist
	virtual HRESULT __stdcall GetClassID(CLSID* pClassID) = 0;

	//IPersistStream
	virtual HRESULT __stdcall IsDirty() R0;
	virtual HRESULT __stdcall Load(IStream* pStm) = 0;
	virtual HRESULT __stdcall Save(IStream* pStm, BOOL fClearDirty) = 0;

	virtual HRESULT __stdcall GetSizeMax(ULARGE_INTEGER* pcbSize) R0;

	//IRTTITypeInfo
	virtual eAbstractType __stdcall What_Am_I() const R0;
	virtual int __stdcall Fetch_ID() const R0;
	virtual void __stdcall Create_ID() RX;

	//INoticeSink
	virtual bool __stdcall INoticeSink_Unknown(DWORD dwUnknown) R0;

	//INoticeSource
	virtual void __stdcall INoticeSource_Unknown() RX;

	//Destructor
	virtual ~AbstractClass() RX;

	//AbstractClass
	virtual void Init() RX;
	virtual void PointerExpired(void* p, bool removed) RX;
	virtual eAbstractType WhatAmI() const = 0;
	virtual int Size() const = 0;
	virtual void CalculateChecksum(Checksummer* pChkSum) const RX;
	virtual int GetOwningHouseIndex() const R0;
	virtual HouseClass* GetOwningHouse() const R0;
	virtual int GetArrayIndex() const R0;
	virtual bool IsDead() const R0;
	virtual CoordStruct* GetCoords(CoordStruct* pCrd) const R0;
	virtual CoordStruct* GetCoords_(CoordStruct* pCrd) const R0;
	virtual bool IsOnFloor() const R0;
	virtual bool IsInAir() const R0;
	virtual CoordStruct* GetCoords__(CoordStruct* pCrd) const R0;
	virtual void Update() RX;

	//Constructor
	AbstractClass()
		{ JMP_THIS(0x410170); }

	//non-virtual
	void AnnounceExpiredPointer(bool removed=true)
		{ JMP_THIS(0x7258D0); }

	CoordStruct GetCoords() const {
		CoordStruct ret;
		return *this->GetCoords(&ret);
	}

	CoordStruct GetCoords_() const {
		CoordStruct ret;
		return *this->GetCoords_(&ret);
	}

	CoordStruct GetCoords__() const {
		CoordStruct ret;
		return *this->GetCoords__(&ret);
	}

	//Operators
	bool operator < (const AbstractClass &rhs) const {
		return this->UniqueID < rhs.UniqueID;
	}

protected:
	AbstractClass(bool) { }	//trick to disable further base class construction!

	//===========================================================================
	//===== Properties ==========================================================
	//===========================================================================

public:

	DWORD UniqueID; // generated by IRTTIInfo::Create_ID through an amazingly simple sequence of return ++ScenarioClass::Instance->UniqueID;
	DWORD AbstractFlags;	//Flags, see ABSFLAGS definitions in GeneralDefinitions.
	DWORD unknown_18;
	LONG RefCount;
	bool bIsDirty;		//For IUnknown.
};

#endif
