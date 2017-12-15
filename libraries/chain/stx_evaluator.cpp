/* (c) 2016, 2017 DECENT Services. For details refers to LICENSE.txt */
/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <graphene/chain/database.hpp>
#include <graphene/chain/stx_evaluator.hpp>
#include <graphene/chain/stx_object.hpp>
#include <graphene/chain/account_object.hpp>
#include <graphene/chain/transaction_detail_object.hpp>

namespace graphene { namespace chain {

void_result stx_evaluation::do_evaluate(const custom_operation& o)
{
   try{

      stx_payload pl;
      o.get_stx_payload(pl);

      auto& idx = _chain_db->get_index_type<account_index>().indices().get<by_id>();
      auto sender_itr = idx.find( pl.Sender );
      auto receiver_itr = idx.find( pl.Receiver );
      FC_ASSERT( pl.Sender != pl.Receiver );
      FC_ASSERT( o.payer == pl.Sender );
      FC_ASSERT( !pl.Data.empty() );
      FC_ASSERT( pl.TransId > 0 );

      return void_result();
   }FC_CAPTURE_AND_RETHROW((o))
}

void_result stx_evaluation::do_apply(const custom_operation& o)
{
   auto& d = *_chain_db;
   stx_payload pl;
   o.get_stx_payload(pl);

   d.create<stx_object>([&o, &pl, &d](stx_object& obj)
    {
       obj.sender  = pl.Sender;
       obj.receiver = pl.Receiver;
       obj.created = d.head_block_time();
       obj.method_type = pl.MethodType;
       obj.transaction_id = pl.TransId;
       obj.data = pl.Data;
    });
   return void_result();
}

} } // graphene::chain
