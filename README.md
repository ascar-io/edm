This is a BSD 3-clause license EDM clean room implementation based on
paper https://arxiv.org/pdf/1411.7955.pdf.

### Build Status
[![Build Status](https://travis-ci.org/ascar-io/edm.svg?branch=master)](https://travis-ci.org/ascar-io/edm)

# Developer Information

We have added an improvement to the IntervalTree algorithm as
described in the paper. When reaching a leaf node whose number of
elements (S) does not equal to K, we use the low + (high-low)*K/S
point as the calculated median instead of low + (high-low)/2. This
improvement led to 77 better median and 23 worse median than the EDM R
implementation using the large_size_sample_sets.

## Test case for IntervalTree

{small,large}_size_sample_sets.csv are for testing IntervalTree. They
contain arrays of randomly generated numbers and the expected median
calculated by using the reference code. They are csv files and the
format is:

```
sample_size,calculated_approx_median,samples
```

Our IntervalTree's calculated approximate median from `samples` should
match `calculated_approx_median`. See
`test_interval_tree_using_samples()` in `edm-test.cpp`. If our
calculated median does not equal the expected median, we calculate
their respective errors from the real median and print "We lost" or "We
won" depends on if our error is larger or smaller.
