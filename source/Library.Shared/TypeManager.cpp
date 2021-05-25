#include "pch.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
    HashMap<RTTI::IdType, TypeManager::TypeInfo> TypeManager::map;

    Signature::Signature(std::string new_name, Datum::DatumTypes new_type, size_t new_size, size_t new_offset) :
        name(std::move(new_name)), type(new_type), size(new_size), storage_offset(new_offset)
    {
    }

    const Vector<Signature> TypeManager::GetSignaturesForType(const RTTI::IdType type)
    {
        size_t signature_count = 0;
        std::deque<const TypeInfo*> queue;
        auto it = map.Find(type);
        while (it != map.end())
        {
            const TypeInfo& type_info = it->second;
            queue.push_front(&type_info);
            signature_count += type_info.signatures.Size();
            it = map.Find(type_info.parent_id_type);
        }

        Vector<Signature> signatures(signature_count);
        for (const TypeInfo* type_info : queue)
        {
            for (const Signature& signature : type_info->signatures)
            {
                signatures.PushBack(signature);
            }
        }

        return signatures;
    }

    Signature* TypeManager::GetSignature(const RTTI::IdType type, const std::string& entry)
    {
        Signature* signature = nullptr;
        Vector<Signature> signatures = GetSignaturesForType(type);
        
        auto found = std::find_if(signatures.begin(), signatures.end(), [&entry](const Signature& signature) { return signature.name == entry; });
        
        if (found != signatures.end())
        {
            signature = const_cast<Signature*>(&(*found));
        }

        return signature;
    }

    void TypeManager::AddType(const RTTI::IdType type, const Vector<Signature>& signatures, RTTI::IdType parent_id)
    {
        if (map.ContainsKey(type))
        {
            throw std::runtime_error("Type already registered.");
        }

        TypeInfo type_info{ signatures, parent_id };

        map.Insert(std::make_pair(type, type_info));
    }
}
