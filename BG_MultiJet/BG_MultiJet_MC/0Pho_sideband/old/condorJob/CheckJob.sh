#!/bin/bash
root -l -q 'findFailedJobs.C("runList_QCD")'
root -l -q 'findFailedJobs.C("runList_GJets")'