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

#include <functional>
#include "graph.hh"

using std::array;
using std::hash;
using std::iterator;
using std::unordered_set;

// ########################### constructor ###########################
DFSGraph::DFSGraph(Tensor *T)
{
}

// ########################### destructor ############################
DFSGraph::~DFSGraph()
{
#warning undefined function
}

// ########################### rebuild ###############################
void DFSGraph::rebuild()
{
#warning undefined function
}

// ########################### tensor_begin ##########################
unordered_set<Tensor*>::const_iterator DFSGraph::tensor_begin()
{
#warning undefined function
  return unordered_set<Tensor*>::const_iterator{};
}

// ########################### tensor_end ############################
unordered_set<Tensor*>::const_iterator DFSGraph::tensor_end()
{
#warning undefined function
  return unordered_set<Tensor*>::const_iterator{};
}

// ########################### vertex_begin ##########################
unordered_set<array<Tensor*,2> >::const_iterator DFSGraph::vertex_begin()
{
#warning undefined function
  return unordered_set<array<Tensor*,2> >::const_iterator{};
}

// ########################### vertex_end ############################
unordered_set<array<Tensor*,2> >::const_iterator DFSGraph::vertex_end()
{
#warning undefined function
  return unordered_set<array<Tensor*,2> >::const_iterator{};
}

// ########################### _dfs ##################################
void DFSGraph::_dfs(Tensor *t)
{
}
