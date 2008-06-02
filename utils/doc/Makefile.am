EXTRA_DIST = opendbx-utils.en.xml


if ENABLEMAN

odbx-sql.1: opendbx-utils.en.xml
	docbook2x-man --solinks opendbx-utils.en.xml

clean:
	for file in `ls odbx*.1`; do rm -f $$file; done

man_MANS = \
	odbx-sql.1

endif
