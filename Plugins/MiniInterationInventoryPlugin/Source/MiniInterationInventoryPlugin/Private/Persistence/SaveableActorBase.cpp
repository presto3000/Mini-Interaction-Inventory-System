// presto3000 Chris Pawlowski All Rights reserved.


#include "MiniInterationInventoryPlugin/Public/Persistence/SaveableActorBase.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"

ASaveableActorBase::ASaveableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASaveableActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!SaveId.IsValid())
	{
		SaveId = FGuid::NewGuid();
	}
}

TArray<uint8> ASaveableActorBase::CaptureSaveState() const
{
	TArray<uint8> Bytes;
	FMemoryWriter Writer(Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(Writer, false);
	Archive.ArIsSaveGame = true;

	const_cast<ASaveableActorBase*>(this)->Serialize(Archive);

	return Bytes;
}

void ASaveableActorBase::RestoreSaveState(const TArray<uint8>& Bytes)
{
	if (Bytes.Num() == 0)
	{
		return;
	}

	FMemoryReader Reader(Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(Reader, false);
	Archive.ArIsSaveGame = true;

	Serialize(Archive);
}
