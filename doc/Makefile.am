EXTRA_DIST = opendbx.en.xml


if ENABLEMAN

odbx_bind.3: opendbx.en.xml
	docbook2x-man --solinks opendbx.en.xml

clean:
	rm odbx_*.3

man_MANS = \
	odbx_bind.3 \
	odbx_bind_simple.3 \
	odbx_capabilities.3 \
	odbx_column_count.3 \
	odbx_column_name.3 \
	odbx_column_type.3 \
	odbx_error.3 \
	odbx_error_type.3 \
	odbx_escape.3 \
	odbx_field_length.3 \
	odbx_field_value.3 \
	odbx_finish.3 \
	odbx_get_option.3 \
	odbx_init.3 \
	odbx_query.3 \
	odbx_result.3 \
	odbx_result_free.3 \
	odbx_row_fetch.3 \
	odbx_rows_affected.3 \
	odbx_set_option.3 \
	odbx_unbind.3

endif