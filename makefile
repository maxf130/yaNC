snapshot_factory_aos.o: snapshot_factory_aos.cc snapshot_factory.h data.h particle.h
	cc -c snapshot_factory_aos.cc

snapshot_factory_soa.o: snapshot_factory_soa.cc snapshot_factory.h data.h particle.h
	cc -c snapshot_factory_soa.cc

clean:
	rm snapshot_factory_soa.o snapshot_factory_aos.o
