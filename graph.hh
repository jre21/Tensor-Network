// Copyright 2013 Jacob Emmert-Aronson
// This file is part of Tensor Network.
//
// Tensor Network is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// Tensor Network is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tensor Network.  If not, see
// <http://www.gnu.org/licenses/>.

#pragma once

#include <array>
#include <iterator>
#include <unordered_set>

// forward declare to avoid dependencies between headers
class Tensor;

// define needed hash function overload
template <>
struct std::hash<std::array<Tensor*,2>>
{
  size_t operator() (std::array<Tensor*,2> arr)
  {
    std::hash<Tensor*> hash;
    return hash(arr[0]) ^ hash(arr[1]);
  }
};

class Graph
{
public:
  virtual ~Graph() {}
  // Reindex all tensors in the graph.  This must be called if the
  // connections between tensors are changed.
  virtual void rebuild() = 0;
  // The following four functions are iterators for use with functions
  // which need to act on either all tensors or all links.  No
  // guarantees are made about ordering.
  virtual std::unordered_set<Tensor*>::const_iterator tensor_begin() = 0;
  virtual std::unordered_set<Tensor*>::const_iterator tensor_end() = 0;
  virtual std::unordered_set<std::array<Tensor*,2> >::const_iterator
  vertex_begin() = 0;
  virtual std::unordered_set<std::array<Tensor*,2> >::const_iterator
  vertex_end() = 0;
};

class DFSGraph : public Graph
{
public:
  // Create the graph of all tensors connected to t.
  DFSGraph(Tensor *t);
  ~DFSGraph();
  // From interface Graph.
  void rebuild() override;
  std::unordered_set<Tensor*>::const_iterator tensor_begin() override;
  std::unordered_set<Tensor*>::const_iterator tensor_end() override;
  std::unordered_set<std::array<Tensor*,2> >::const_iterator
  vertex_begin() override;
  std::unordered_set<std::array<Tensor*,2> >::const_iterator
  vertex_end() override;
protected:
  // function which is called recursively to find all connected
  // tensors via depth-first search
  void _dfs(Tensor *t);
private:
  // tensors which have not yet been processed
  std::unordered_set<Tensor*> _discovered;
  // tensors which belong to the graph
  std::unordered_set<Tensor*> _nodes;
  // vertices connecting tensors
  std::unordered_set<std::array<Tensor*,2> > _vertices;
};
