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
#include "tensor.hh"

using std::hash;
using std::iterator;
using std::unordered_set;

// ########################### GraphEdge ################################
// ########################### operator== ############################
bool GraphEdge::operator== (const GraphEdge &v) const
{
  return input_tensor == v.input_tensor && input_num == v.input_num
    && output_tensor == v.output_tensor && output_num == v.output_num;
}

// ########################### operator!= ############################
bool GraphEdge::operator!= (const GraphEdge &v) const
{
  return input_tensor != v.input_tensor || input_num != v.input_num
    || output_tensor != v.output_tensor || output_num != v.output_num;
}


// ########################### DFSGraph ##############################
// ########################### constructor ###########################
DFSGraph::DFSGraph(Tensor *T)
{
}

// ########################### destructor ############################
DFSGraph::~DFSGraph()
{
#warning undefined function
}

// ########################### vertices ##############################
size_t DFSGraph::vertices()
{
#warning undefined function
  return 0;
}

// ########################### edges #################################
size_t DFSGraph::edges()
{
#warning undefined function
  return 0;
}

// ########################### vertex_begin ##########################
unordered_set<Tensor*>::const_iterator DFSGraph::vertex_begin()
{
#warning undefined function
  return unordered_set<Tensor*>::const_iterator{};
}

// ########################### vertex_end ############################
unordered_set<Tensor*>::const_iterator DFSGraph::vertex_end()
{
#warning undefined function
  return unordered_set<Tensor*>::const_iterator{};
}

// ########################### edge_begin ############################
unordered_set<GraphEdge>::const_iterator DFSGraph::edge_begin()
{
#warning undefined function
  return unordered_set<GraphEdge>::const_iterator{};
}

// ########################### edge_end ##############################
unordered_set<GraphEdge>::const_iterator DFSGraph::edge_end()
{
#warning undefined function
  return unordered_set<GraphEdge>::const_iterator{};
}

// ########################### _dfs ##################################
void DFSGraph::_dfs(Tensor *t)
{
}
