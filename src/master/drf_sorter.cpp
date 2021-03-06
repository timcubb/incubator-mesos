/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "logging/logging.hpp"

#include "master/drf_sorter.hpp"

using std::list;
using std::set;
using std::string;


namespace mesos {
namespace internal {
namespace master {
namespace allocator {

bool DRFComparator::operator () (
    const Client& client1,
    const Client& client2)
{
  if (client1.share == client2.share) {
    return client1.name < client2.name;
  }
  return client1.share < client2.share;
}


void DRFSorter::add(const string& name)
{
  Client client;
  client.name = name;
  client.share = 0;
  clients.insert(client);

  allocations[name] = Resources::parse("");
}


void DRFSorter::remove(const string& name)
{
  set<Client, DRFComparator>::iterator it = find(name);

  if (it != clients.end()) {
    clients.erase(it);
  }

  allocations.erase(name);
}


void DRFSorter::activate(const string& name)
{
  CHECK(allocations.contains(name));

  Client client;
  client.name = name;
  client.share = calculateShare(name);
  clients.insert(client);
}


void DRFSorter::deactivate(const string& name)
{
  set<Client, DRFComparator>::iterator it = find(name);

  if (it != clients.end()) {
    clients.erase(it);
  }
}


void DRFSorter::allocated(
    const string& name,
    const Resources& resources)
{
  allocations[name] += resources;

  // If the total resources have changed, we're going to
  // recalculate all the shares, so don't bother just
  // updating this client.
  if (!dirty) {
    update(name);
  }
}


Resources DRFSorter::allocation(
    const string& name)
{
  return allocations[name];
}


void DRFSorter::unallocated(
    const string& name,
    const Resources& resources)
{
  allocations[name] -= resources;

  if (!dirty) {
    update(name);
  }
}


void DRFSorter::add(const Resources& _resources)
{
  resources += _resources;

  // We have to recalculate all shares when the total resources
  // change, but we put it off until sort is called
  // so that if something else changes before the next allocation
  // we don't recalculate everything twice.
  dirty = true;
}


void DRFSorter::remove(const Resources& _resources)
{
  resources -= _resources;
  dirty = true;
}


list<string> DRFSorter::sort()
{
  if (dirty) {
    set<Client, DRFComparator> temp;

    set<Client, DRFComparator>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++) {
      Client client;
      client.name = (*it).name;
      client.share = calculateShare((*it).name);
      temp.insert(client);
    }

    clients = temp;
  }

  list<string> ret;

  set<Client, DRFComparator>::iterator it;
  for (it = clients.begin(); it != clients.end(); it++) {
    ret.push_back((*it).name);
  }

  return ret;
}


bool DRFSorter::contains(const string& name)
{
  return allocations.contains(name);
}


int DRFSorter::count()
{
  return allocations.size();
}

void DRFSorter::update(const string& name)
{
  set<Client, DRFComparator>::iterator it;
  it = find(name);
  clients.erase(it);

  Client client;
  client.name = name;
  client.share = calculateShare(name);
  clients.insert(client);
}


double DRFSorter::calculateShare(const string& name)
{
  double share = 0;

  // TODO(benh): This implementaion of "dominant resource fairness"
  // currently does not take into account resources that are not
  // scalars.

  foreach (const Resource& resource, resources) {
    if (resource.type() == Value::SCALAR) {
      double total = resource.scalar().value();

    if (total > 0) {
      Value::Scalar none;
      const Value::Scalar& scalar =
        allocations[name].get(resource.name(), none);

      share = std::max(share, scalar.value() / total);
    }
  }
}

  return share;
}


set<Client, DRFComparator>::iterator DRFSorter::find(const string& name)
{
  set<Client, DRFComparator>::iterator it;
  for (it = clients.begin(); it != clients.end(); it++) {
    if (name == (*it).name) {
      break;
    }
  }

  return it;
}

} // namespace allocator {
} // namespace master {
} // namespace internal {
} // namespace mesos {
