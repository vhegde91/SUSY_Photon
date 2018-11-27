#!/bin/bash
echo "dependent_variables:"
awk -F '&' 'NR==1{print"- header: {name: '"'"'",$2,"'"'"'}"}' cutFlowTable.txt
echo "  qualifiers:"
echo "  - {name: '$\sqrt s$', units: GeV, value: 13000.0}"
echo "  - {name: Integrated luminosity, units: '\$fb^{-1}$', value: 35.9}"
echo "  - {name: '\$m_{\tilde{g}}$', units: 'GeV', value: 1800}"

echo "  values:"
awk -F '&' 'NR!=1{print"  - {value: '"'"'",$2,"'"'"'}"}' cutFlowTable.txt

awk -F '&' 'NR==1{print"- header: {name: '"'"'",$3,"'"'"'}"}' cutFlowTable.txt
echo "  values:"
awk -F '&' 'NR!=1{print"  - {value: '"'"'",$3,"'"'"'}"}' cutFlowTable.txt

awk -F '&' 'NR==1{print"- header: {name: '"'"'",$4,"'"'"'}"}' cutFlowTable.txt
echo "  values:"
awk -F '&' 'NR!=1{print"  - {value: '"'"'",$4,"'"'"'}"}' cutFlowTable.txt

awk -F '&' 'NR==1{print"- header: {name: '"'"'",$5,"'"'"'}"}' cutFlowTable.txt
echo "  values:"
awk -F '&' 'NR!=1{print"  - {value: '"'"'",$5,"'"'"'}"}' cutFlowTable.txt

awk -F '&' 'NR==1{print"- header: {name: '"'"'",$6,"'"'"'}"}' cutFlowTable.txt
echo "  values:"
awk -F '&' 'NR!=1{print"  - {value: '"'"'",$6,"'"'"'}"}' cutFlowTable.txt


echo "independent_variables:"
awk -F '&' 'NR==1{print"- header: {name: '"'"'",$1,"'"'"'}"}' cutFlowTable.txt
echo "  values:"
awk -F '&' 'NR!=1{print"  - {value: ",$1,"}"}' cutFlowTable.txt

########
#echo "- header: {name: '\$m_{\tilde{g}} = 1800$ ', units: GeV}"
#echo "  values:"
#awk -F '&' '{print"  - {value: '"'"'",$3,"'"'"'}"}' cutFlowTable.txt

#echo "- header: {name: }"

#echo "- header: {name: '\$m_{\tilde{g}} = 1800$ ', units: GeV}"
#echo "  values:"
#awk -F '&' '{print"  - {value: '"'"'",$4,"'"'"'}"}' cutFlowTable.txt

#echo "- header: {name: '\$m_{\tilde{g}} = 1800$ ', units: GeV}"
#echo "  values:"
#awk -F '&' '{print"  - {value: '"'"'",$5,"'"'"'}"}' cutFlowTable.txt

#echo "- header: {name: '\$m_{\tilde{g}} = 1800$ ', units: GeV}"
#echo "  values:"
#awk -F '&' '{print"  - {value: '"'"'",$6,"'"'"'}"}' cutFlowTable.txt


#echo "independent_variables:"
#echo "- header: {name: Cut no.}"
#echo "  values:"
#awk -F '&' '{print"  - {value: ",$1,"}"}' cutFlowTable.txt

