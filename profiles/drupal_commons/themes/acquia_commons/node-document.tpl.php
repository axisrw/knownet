 <?php if ($page == 0): ?>
    <h2 class="title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
    <?php endif; ?>
<span class="submitted">Document <?php print $submitted ?></span><?php print $terms; ?>
<div class="content clearfix"><?php print $content ?> </div>

 <br>
<div class="links">
      <?php print $links; ?>
 </div>
 <hr class="hr">