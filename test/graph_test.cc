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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../tensor.hh"
#include "../graph.hh"

using std::unordered_set;

TEST(GraphTest,EdgeEquality) {
  GraphEdge e0 {nullptr, 0, nullptr, 0};
  GraphEdge e1 {nullptr, 0, nullptr, 0};
  GraphEdge e2 {nullptr, 1, nullptr, 0};
  EXPECT_EQ(e0, e1);
  EXPECT_NE(e0, e2);
}

// Test a relatively simple graph with this structure:
// 0-1
// |/|
// 2-3-4
TEST(GraphTest,BuildGraph) {
  Tensor *t0 = new ConcreteTensor(2,2,0,0),
    *t1 = new ConcreteTensor(2,2,0,0),
    *t2 = new ConcreteTensor(2,2,0,0),
    *t3 = new ConcreteTensor(2,2,0,0),
    *t4 = new ConcreteTensor(2,2,0,0);

  // set up structure
  t0->set_input(0,t1,0);
  t0->set_input(1,t2,1);
  t1->set_input(0,t3,1);
  t2->set_output(0,t3,1);
  t3->set_input(1,t4,0);
  t1->set_output(1,t2,0);

  // build and check graph
  DFSGraph graph{t0};

  unordered_set<Tensor*> vertices(graph.vertex_begin(), graph.vertex_end());
  unordered_set<GraphEdge> edges(graph.edge_begin(), graph.edge_end());

  EXPECT_EQ(5, vertices.size());
  EXPECT_EQ(1, vertices.count(t0));
  EXPECT_EQ(1, vertices.count(t1));
  EXPECT_EQ(1, vertices.count(t2));
  EXPECT_EQ(1, vertices.count(t3));
  EXPECT_EQ(1, vertices.count(t4));

  EXPECT_EQ(6, edges.size());
  EXPECT_EQ(1, edges.count(GraphEdge{t0,0,t1,0}));
  EXPECT_EQ(1, edges.count(GraphEdge{t0,1,t2,1}));
  EXPECT_EQ(1, edges.count(GraphEdge{t1,0,t3,1}));
  EXPECT_EQ(1, edges.count(GraphEdge{t3,1,t2,0}));
  EXPECT_EQ(1, edges.count(GraphEdge{t3,1,t4,0}));
  EXPECT_EQ(1, edges.count(GraphEdge{t2,0,t1,1}));

  delete t0;
  delete t1;
  delete t2;
  delete t3;
  delete t4;
}
