for SodaCost in 5 10
do
for NumStudents in 3 6
do  
for MaxPurchases in 1 4
do  
for NumVendingMachines in 1 3
do  
for MaxStockPerFlavour in 3 9
do  
for MaxShippedPerFlavour in 5 11
do 
for TimeBetweenShipments in 2 9
do 
for GroupoffDelay in 3 11
do 
for ParentalDelay in 4 10
do  
for NumCouriers in 1 6
do  
    echo "" > linda.config
    echo SodaCost $SodaCost >> linda.config
    echo NumStudents $NumStudents >> linda.config
    echo MaxPurchases $MaxPurchases >> linda.config
    echo NumVendingMachines $NumVendingMachines >> linda.config
    echo MaxStockPerFlavour $MaxStockPerFlavour >> linda.config
    echo MaxShippedPerFlavour $MaxShippedPerFlavour >> linda.config
    echo TimeBetweenShipments $TimeBetweenShipments >> linda.config
    echo GroupoffDelay $GroupoffDelay >> linda.config
    echo ParentalDelay $ParentalDelay >> linda.config
    echo NumCouriers $NumCouriers >> linda.config
    ./soda linda.config 2
done
done
done
done
done
done
done
done
done
done
