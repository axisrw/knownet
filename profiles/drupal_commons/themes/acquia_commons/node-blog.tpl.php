 <?php if ($page == 0): ?>
    <h2 class="title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
    <?php endif; ?>
<span class="submitted">Blog Post <?php print $submitted ?></span><?php print $terms; ?>
<br><br>

<div class="content clearfix"><?php print_r($content) ?> </div>

 <br>
<div class="links">
      <?php print $links; ?>
 </div>
 <hr class="hr">