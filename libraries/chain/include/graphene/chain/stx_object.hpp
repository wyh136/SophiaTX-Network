#pragma once

#include <graphene/chain/protocol/types.hpp>
#include <graphene/db/object.hpp>
#include <graphene/db/generic_index.hpp>

#include <fc/reflect/reflect.hpp>
#include <fc/time.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain{

class stx_object : public graphene::db::abstract_object< stx_object >
      {
      public:
         static const uint8_t space_id = implementation_ids;
         static const uint8_t type_id  = impl_stx_object_type;

         account_id_type sender;
         account_id_type receiver;
         string method_type;
         uint64_t transaction_id;
         string data;
         time_point_sec created;
      };

      struct by_sender;
      struct by_receiver;
      struct by_transaction_id;
      struct by_sender_id;
      struct by_receiver_id;

      typedef multi_index_container<
      stx_object,
      indexed_by<
         ordered_unique< tag< by_id>,
            member< object, object_id_type, &object::id >
         >,
         ordered_non_unique< tag< by_sender>,
            member<stx_object, account_id_type, &stx_object::sender>
         >,
         ordered_non_unique< tag< by_receiver>,
            member<stx_object, account_id_type, &stx_object::receiver>
         >,
         ordered_unique< tag< by_transaction_id>,
            member<stx_object, uint64_t, &stx_object::transaction_id>
         >,
         ordered_unique< tag< by_sender_id>,
            composite_key< stx_object,
               member<stx_object, account_id_type, &stx_object::sender>,
               member<object, object_id_type, &object::id>
            >
         >,
         ordered_unique< tag< by_receiver_id>,
            composite_key< stx_object,
               member<stx_object, account_id_type, &stx_object::receiver>,
               member<object, object_id_type, &object::id>
            >
         >
      >
      >stx_object_multi_index_type;

      typedef generic_index< stx_object, stx_object_multi_index_type > stx_index;

}}

FC_REFLECT_DERIVED(graphene::chain::stx_object, (graphene::db::object), (sender)(receiver)(method_type)(transaction_id)(data)(created) )
