This is a BSD 3-clause license EDM clean room implementation based on
paper https://arxiv.org/pdf/1411.7955.pdf.

### Build Status
[![Build Status](https://travis-ci.org/ascar-io/edm.svg?branch=master)](https://travis-ci.org/ascar-io/edm)

# Developer Information

## Test cases

{small,large}_size_sample_sets.csv are for testing IntervalTree. They are csv files and the format is:

```
sample_size,calculated_approx_median,samples
```

IntervalTree's calculated approximate median from `samples` should
match `calculated_approx_median`. See
`test_interval_tree_using_samples()` in `edm-test.cpp`.
