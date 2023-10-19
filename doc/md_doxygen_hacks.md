@page md_doxygen_hacks Markdown doxygen hacks

# Markdown doxygen hacks

I try to maintain clear and well documented git repository so there is
a `README.md` file in almost each sub-directory.

We also use doxygen to generate a `refman.pdf` where the first chapter
is generated from repository's `.md` files.

That requires the use of doxygen tags in the `.md` files but it taints
them rendered in gitlab.

We also require to hide some redundant unsupported markdown sections
from the doxygen pdf output.


# Hide a section from both outputs

HTML comment tags in markdown are supported by both doxygen and gitlab
so they seems to be useless.


# Hide a section from the gitlab render only

With general markdown render engine's basic security mechanisms in mind,
I tried to use the `<script>` html tag and "bingo" ! As pdf is not
vulnerable to html injection, doxygen and pdflatex do not
handle it and it is only rendered as is.
<!-- @cond COMMENT --><script><!--</script>
    <script>

    Content hidden from gitlab

    </script>
<script>--></script><script><!-- @endcond -->
    @verbatim
    <script>

    Content hidden from gitlab

    </script>
    @endverbatim
<!-- @cond COMMENT --></script><!-- @endcond -->
In pdf it will be rendered:

`<script>`

Content hidden from gitlab
`</script>`

When we are using `<script>` tags to hide some content from gitlab they
still appear in pdf output so I have to find a way to hide them and also
to hide "File organization" tables with markdown links.

I found that I can use <!-- @cond COMMENT --><script><!--
</script>@<script></script>cond<script>
--></script><script><!--
@endcond -->`@``cond` <!--
@cond COMMENT --></script><!-- @endcond -->
and <!-- @cond COMMENT --><script><!--
</script>@<script></script>endcond<script>
--></script><script><!--
@endcond -->`@``endcond` <!--
@cond COMMENT --></script><!-- @endcond -->
doxygen tags to hide them from pdf output but how to hide them from
gitlab output ? I can enclose them in html comment tag as this:
<!-- @cond COMMENT --><script><!--</script>
    <!-- @cond COMMENT --><script><!-- @endcond -->

    Content hidden in gitlab

    <!-- @cond COMMENT --></script><!-- @endcond -->
<script>--></script><script><!-- @endcond -->
    @verbatim
    <!-- @cond COMMENT --><script><!-- @endcond -->

    Content hidden in gitlab

    <!-- @cond COMMENT --></script><!-- @endcond -->
    @endverbatim
<!-- @cond COMMENT --></script><!-- @endcond -->


# Hide a section from the doxygen pdf output only

Now, since content between `<script>` and `</script>` will be removed from
gitlab output prior to html comment tags, we could combine them to hide
the content from the pdf output:
<!-- @cond COMMENT --><script><!--</script>
    <script><!--</script>

    This section appear in gitlab because html comment begin tag was removed
    berfore to be parsed.

    <script>--></script>
<script>--></script><script><!-- @endcond -->
    @verbatim
    <script><!--</script>

    This section appear in gitlab because html comment begin tag was removed
    berfore to be parsed.

    <script>--></script>
    @endverbatim
<!-- @cond COMMENT --></script><!-- @endcond -->

But this result in a `<script></script>` line in pdf output so we need
to hide it:
<!-- @cond COMMENT --><script><!--</script>
    <!-- @cond COMMENT --><script><!--</script>

    This section appear in gitlab because html comment begin tag was removed
    berfore to be parsed.

    <script>--></script><!-- @endcond -->
<script>--></script><script><!-- @endcond -->
    @verbatim
    <!-- @cond COMMENT --><script><!--</script>

    This section appear in gitlab because html comment begin tag was removed
    berfore to be parsed.

    <script>--></script><!-- @endcond -->
    @endverbatim
<!-- @cond COMMENT --></script><!-- @endcond -->


# Example
<!-- @cond COMMENT --><script><!--</script>
    <!-- @cond COMMENT --><script><!--</script>

    This section only appear in gitlab so [markup link](url) to url will
    be properly rendered.

    <script>--></script><script><!-- @endcond -->

    This section only appear in pdf so @subpage label will be properly
    renderd.

    <!-- @cond COMMENT --></script><!-- @endcond -->
<script>--></script><script><!-- @endcond -->
    @verbatim
    <!-- @cond COMMENT --><script><!--</script>

    This section only appear in gitlab so [markup link](url) to url will
    be properly rendered.

    <script>--></script><script><!-- @endcond -->

    This section only appear in pdf so @subpage label will be properly
    renderd.

    <!-- @cond COMMENT --></script><!-- @endcond -->
    @endverbatim
<!-- @cond COMMENT --></script><!-- @endcond -->

As you can see <!-- @cond COMMENT --><script><!--
</script> `<!-- @endcond --><!-- @cond COMMENT -->` <script>
--></script><script><!-- @endcond -->
@verbatim <!-- @endcond --><!-- @cond COMMENT --> @endverbatim
<!-- @cond COMMENT --></script><!-- @endcond --> have been omitted
between <!-- @cond COMMENT --><script><!--
</script>
`<<script></script>script>--><<script></script>/script>`
<script>--></script><script><!-- @endcond -->
`<script>--></script>`
<!-- @cond COMMENT --></script><!-- @endcond --> and <!-- @cond COMMENT --><script><!--
</script>
`<<script></script>script>`
<script>--></script><script><!-- @endcond -->
`<script>`
<!-- @cond COMMENT --></script><!-- @endcond --> as it is useless here.


# Warning: it can become hell

UGLY HACK! isn't it ?

If you want to see how much, have a look at this file source code.
