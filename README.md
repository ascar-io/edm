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

{small,large}_size_sample_sets.csv are for testing IntervalTree.  They
contain arrays of randomly generated numbers and the expected median
calculated by using the reference code.You can run our implementation
of IntervalTree against these test cases:

```
./edm-test large_size_sample_sets.csv
./edm-test small_size_sample_sets.csv
```

Or with `-v` to show the results of each test case:

```
./edm-test -v large_size_sample_sets.csv
./edm-test -v small_size_sample_sets.csv
```

The format of these csv files is:

```
sample_size,calculated_approx_median,samples
```

We calculate the error between our calculated median and the real
median to the error between reference code's calculated median and the
real median and print "We lost" or "We won" depends on if our error is
larger or smaller.
